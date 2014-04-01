/*
 *  Copyright 2010 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  This file is part of the WSF Staff project.
 *  Please, visit http://code.google.com/p/staff for more information.
 */

#include <string>
#include <map>
#include <exception>
#include <staff/utils/Log.h>
#include <staff/utils/SharedPtr.h>
#include <staff/utils/tostring.h>
#include <staff/utils/fromstring.h>
#include <staff/utils/stringutils.h>
#include <staff/common/Attribute.h>
#include <staff/common/DataObject.h>
#include <staff/xml/Attribute.h>
#include <staff/xml/Element.h>
#include "DataSource.h"
#include "Executor.h"
#include "Provider.h"
#include "ScriptExecuter.h"

namespace staff
{
namespace das
{
  struct Var
  {
    DataType tType;
    std::string sValue;
    staff::DataObject tdoValue;
  };

  LogStream& operator<<(LogStream& rStream, const StringList& rList)
  {
    StringList::const_iterator itItem = rList.begin();
    if (itItem != rList.end())
    {
      rStream << *itItem;
      ++itItem;
      for (; itItem != rList.end(); ++itItem)
      {
        rStream << ", " << *itItem;
      }
    }
    return rStream;
  }

  class ScriptExecuter::ScriptExecuterImpl
  {
  public:
    typedef std::map<std::string, Var> VarMap;

    //! list of executors
    typedef std::map<std::string, PExecutor> ExecutorsMap;

  public:
    ScriptExecuterImpl(const DataSource& rDataSource, Providers& rstProviders):
      m_rDataSource(rDataSource), m_rstProviders(rstProviders), m_bReturn(false)
    {
    }

    void ProcessSequence(const DataObject& rdoContext, const xml::Element& rScript,
                                         const DataType& rReturnType, DataObject& rdoResult)
    {
      for (const xml::Element* pOperator = rScript.GetFirstChildElement();
          pOperator && !m_bReturn; pOperator = pOperator->GetNextSiblingElement())
      {
        const std::string& sOperator = pOperator->GetName();

        if (sOperator == "var")
        {
          ProcessVar(rdoContext, *pOperator);
        }
        else
        if (sOperator == "foreach")
        {
          ProcessForeach(rdoContext, *pOperator, rReturnType, rdoResult);
        }
        else
        if (sOperator == "ifeq")
        {
          ProcessIfeq(rdoContext, *pOperator, rReturnType, rdoResult, true);
        }
        else
        if (sOperator == "ifneq")
        {
          ProcessIfeq(rdoContext, *pOperator, rReturnType, rdoResult, false);
        }
        else
        if (sOperator == "if")
        {
          ProcessIf(rdoContext, *pOperator, rReturnType, rdoResult);
        }
        else
        if (sOperator == "switch")
        {
          ProcessSwitch(rdoContext, *pOperator, rReturnType, rdoResult);
        }
        else
        if (sOperator == "return")
        {
          ProcessReturn(rdoContext, *pOperator, rReturnType, rdoResult);
          m_bReturn = true;
          return;
        }
        else
        if (sOperator == "execute")
        {
          ProcessExecute(rdoContext, *pOperator, rReturnType, rdoResult);
        }
        else
        if (sOperator == "log")
        {
          ProcessLog(rdoContext, *pOperator);
        }
        else
        if (sOperator == "fault")
        {
          ProcessFault(rdoContext, *pOperator);
        }
        else
        if (sOperator == "trycatch")
        {
          ProcessTryCatch(rdoContext, *pOperator, rReturnType, rdoResult);
        }
        else
        {
          STAFF_THROW_ASSERT("Invalid operator: [" + sOperator + "]");
        }
      }
    }

    const std::string& GetProviderId(const xml::Element& rScript) const
    {
      const xml::Element* pElem = &rScript;
      const xml::Attribute* pAttr = NULL;
      while (pElem)
      {
        const std::string& sNodeName = pElem->GetName();
        if (sNodeName == "operation")
        {
          break;
        }
        else
        if (sNodeName == "execute" || sNodeName == "script")
        {
          pAttr = pElem->FindAttribute("providerid");
          if (pAttr)
          {
            return pAttr->GetValue();
          }
        }
        else
        if (sNodeName == "provider")
        {
          pAttr = pElem->FindAttribute("id");
          if (pAttr)
          {
            return pAttr->GetValue();
          }
        }
        pElem = pElem->GetParent();
      }

      return m_rstProviders.sDefaultId;
    }

    void ProcessExecute(const DataObject& rdoContext, const xml::Element& rScript,
                        const DataType& rReturnType, DataObject& rdoResult)
    {
      std::string sExecute = rScript.GetValue();
      LogDebug1() << "Query is [" + sExecute + "]";

      const std::string& sProviderId = GetProviderId(rScript);

      PExecutor& rpExec = m_mExec[sProviderId];
      if (!rpExec.Get())
      {
        ProvidersMap::iterator itProvider = m_rstProviders.mProviders.find(sProviderId);
        STAFF_ASSERT(itProvider != m_rstProviders.mProviders.end(), "Provider with id=["
                     + sProviderId + "] does not exists");

        rpExec = itProvider->second->GetExecutor();
        STAFF_ASSERT(rpExec.Get(), "Couldn't get Executor");
      }

      switch (rpExec->GetType())
      {
        case IExecutor::TypeRaw:
        {
          sExecute = Eval(rdoContext, sExecute);
          LogDebug1() << "Query with params [" + sExecute + "]";

          IRawExecutor* pExec = static_cast<IRawExecutor*>(rpExec.Get());
          pExec->Execute(sExecute, rdoContext, rReturnType, rdoResult);
          break;
        }

        case IExecutor::TypeQuery:
        {
          StringList lsParams;
          sExecute = Eval(rdoContext, sExecute, &lsParams);
          LogDebug1() << "Query with params [" + sExecute + "]; Params: (" << lsParams << ")";

          IQueryExecutor* pExec = static_cast<IQueryExecutor*>(rpExec.Get());

          pExec->Execute(sExecute, lsParams);

          bool bPartial = false;
          const xml::Attribute* pAttr = rScript.FindAttribute("partial");
          if (pAttr)
          {
            const std::string& sValue = pAttr->GetValue();
            if (sValue == "true")
            {
              bPartial = true;
            }
            else
            {
              STAFF_ASSERT(sValue == "false", "Invalid value of \"partial\" attribute. "
                           "Valid values are: \"true\", \"false\"(default)");
            }
          }


          StringList lsResult;

          if (rReturnType.eType == DataType::Generic)
          {
            if (pExec->GetNextResult(lsResult))
            {
              STAFF_ASSERT(lsResult.size() == 1,
                           "The number of fields in result do not match ["
                           + rReturnType.sName + " " + rReturnType.sType + "]: " +
                           ToString(lsResult.size()) + " butexpected: 1");
              const std::string& sValue = lsResult.front();
              if (sValue == STAFF_DAS_NULL_VALUE)
              {
                rdoResult.SetNil();
              }
              else
              {
                rdoResult.SetText(sValue);
              }
            }
          }
          else
          if (rReturnType.eType == DataType::Struct)
          {
            if (pExec->GetNextResult(lsResult))
            {
              STAFF_ASSERT((bPartial && lsResult.size() > 0)
                           || lsResult.size() == rReturnType.lsChilds.size(),
                           "The number of fields in result do not match ["
                           + rReturnType.sName + " " + rReturnType.sType + "]: " +
                           ToString(lsResult.size()) + " but expected: "
                           + ToString(rReturnType.lsChilds.size()));

              StringList::const_iterator itResult = lsResult.begin();
              for (DataTypesList::const_iterator itType = rReturnType.lsChilds.begin();
                  itType != rReturnType.lsChilds.end() &&
                   itResult != lsResult.end(); ++itType, ++itResult)
              {
                if (*itResult == STAFF_DAS_NULL_VALUE)
                {
                  rdoResult.CreateChild(itType->sName).SetNil();
                }
                else
                {
                  rdoResult.CreateChild(itType->sName).SetText(*itResult);
                }
              }
            }
          }
          else
          if (rReturnType.eType == DataType::DataObject ||
              (rReturnType.eType == DataType::List &&
               rReturnType.lsChilds.front().eType == DataType::DataObject))
          {
            StringList lsFieldsNames;
            pExec->GetFieldsNames(lsFieldsNames);
            while (pExec->GetNextResult(lsResult))
            {
              staff::DataObject tdoItem = rdoResult.CreateChild("Item");

              for (StringList::const_iterator itResult = lsResult.begin(), itName = lsFieldsNames.begin();
                   itResult != lsResult.end(); ++itResult, ++itName)
              {
                if (*itResult == STAFF_DAS_NULL_VALUE)
                {
                  tdoItem.CreateChild(*itName).SetNil();
                }
                else
                {
                  tdoItem.CreateChild(*itName).SetText(*itResult);
                }
              }
            }
          }
          else
          if (rReturnType.eType == DataType::List) // ---------------- list ----------------------------
          {
            const DataType& rItemType = rReturnType.lsChilds.front();
            if (rItemType.eType == DataType::Generic) // list of generics
            {
              if (pExec->GetNextResult(lsResult))
              {
                STAFF_ASSERT(lsResult.size() == 1,
                             "The number of fields in result do not match ["
                             + rItemType.sName + " " + rItemType.sType + "]: " +
                             ToString(lsResult.size()) + " but expected: 1");
                do
                {
                  const std::string& sResult = lsResult.front();
                  if (sResult == STAFF_DAS_NULL_VALUE)
                  {
                    rdoResult.CreateChild("Item").SetNil();
                  }
                  else
                  {
                    rdoResult.CreateChild("Item").SetText(sResult);
                  }
                }
                while (pExec->GetNextResult(lsResult));
              }
            }
            else
            if (rItemType.eType == DataType::Struct) // list of structs
            {
              if (pExec->GetNextResult(lsResult))
              {
                STAFF_ASSERT((bPartial && lsResult.size() > 0)
                             || lsResult.size() == rItemType.lsChilds.size(),
                             "The number of fields in result do not match ["
                             + rItemType.sName + " " + rItemType.sType + "]: " +
                             ToString(lsResult.size()) + " but expected: "
                             + ToString(rItemType.lsChilds.size()));
                do
                {
                  staff::DataObject tdoItem = rdoResult.CreateChild("Item");

                  StringList::const_iterator itResult = lsResult.begin();
                  for (DataTypesList::const_iterator itType = rItemType.lsChilds.begin();
                      itType != rItemType.lsChilds.end() && 
                      itResult != lsResult.end(); ++itType, ++itResult)
                  {
                    if (*itResult == STAFF_DAS_NULL_VALUE)
                    {
                      tdoItem.CreateChild(itType->sName).SetNil();
                    }
                    else
                    {
                      tdoItem.CreateChild(itType->sName).SetText(*itResult);
                    }
                  }
                }
                while (pExec->GetNextResult(lsResult));
              }
            }
            else
            {
              STAFF_THROW_ASSERT("Unsupported list item type: " + rReturnType.sType);
            }
          }
          else
          if (rReturnType.eType != DataType::Void)
          {
            STAFF_THROW_ASSERT("Unsupported return type: " + rReturnType.sType);
          }
          break;
        }

        default:
          STAFF_THROW_ASSERT("Unknown executor type");
      }

      const xml::Attribute* pAttr = rScript.FindAttribute("result");
      if (pAttr)
      {
        const std::string& sValue = pAttr->GetValue();
        if (sValue == "required")
        {
          STAFF_ASSERT(!rdoResult.IsTextNull() || !rdoResult.FirstChild().IsNull(),
                       "Empty result, when result is required");
        }
        else
        {
          STAFF_ASSERT(sValue == "optional", "Invalid \"result\" attribute value. "
                       "Valid values are: \"optional\"(default), \"required\"");
        }
      }

      pAttr = rScript.FindAttribute("trim");
      if (pAttr && pAttr->GetValue() == "true" && !rdoResult.IsTextNull())
      {
        std::string sText = rdoResult.GetText();
        StringTrim(sText);
        rdoResult.SetText(sText);
      }

    }

    void ProcessVar(const DataObject& rdoContext, const xml::Element& rScript)
    {
      std::string sVarName = rScript.GetAttributeValue("name");
      std::string sVarPath;

      std::string::size_type nPos = sVarName.find_first_of('.');
      if (nPos != std::string::npos)
      {
        sVarPath = sVarName.substr(nPos + 1);
        sVarName.erase(nPos);
      }

      Var& rVar = m_mVars[sVarName];

      DataType tType;

      // get var type
      const xml::Attribute* pAttrType = rScript.FindAttribute("type");
      if (pAttrType)
      {
        const std::string& sType = pAttrType->GetValue();

        const DataType* pType = m_rDataSource.FindType(sType);

        if (pType)
        { // datasource's type
          tType = *pType;
        }
        else
        { // parse type
          tType.sType = sType;
          if (sType == "DataObject")
          {
            tType.eType = DataType::DataObject;
          }
          else
          { // accept as generic
            tType.eType = DataType::Generic;
          }
        }

        // only set type for root, children are always DataObject
        if (sVarPath.empty())
        {
          // reset variable in case of giving type for root
          rVar.tdoValue.Detach();
          rVar.tType = tType;
        }
      }



      // if "value" attr is given, create variable of generic type
      // else create variable of DataObject type

      const xml::Attribute* pAttrValue = rScript.FindAttribute("value");
      if (pAttrValue)
      {
        const std::string& sValue = Eval(rdoContext, pAttrValue->GetValue());
        if (tType.eType == DataType::Void)
        {
          tType.eType = DataType::Generic;
        }

        // set value for root
        if (sVarPath.empty())
        {
          rVar.sValue = sValue;
        }
        else
        {
          DataObject tdoValue;
          CreateDataObject(rVar.tdoValue, sVarName, sVarPath, tdoValue);
          tdoValue.SetText(sValue);
        }
      }
      else
      {
        if (tType.eType == DataType::Void)
        {
          tType.eType = DataType::DataObject;
        }

        DataObject tdoValue;
        CreateDataObject(rVar.tdoValue, sVarName, sVarPath, tdoValue);

        ProcessSequence(rdoContext, rScript, tType, tdoValue);
        if (tType.eType == DataType::Generic)
        {
          rVar.sValue = tdoValue.GetText();
          rVar.tdoValue.Detach();
          if (tType.sType != "string")
          {
            StringTrim(rVar.sValue);
          }
        }
      }
    }


    void ProcessForeach(const DataObject& rdoContext, const xml::Element& rScript,
                        const DataType& rReturnType, DataObject& rdoResult)
    {
      DataObject tdoElement = rdoContext;

      const xml::Attribute* pAttrElementName = rScript.FindAttribute("element");
      if (pAttrElementName)
      {
        GetChild(tdoElement, pAttrElementName->GetValue(), tdoElement);
      }

      for (DataObject::Iterator itChild = tdoElement.Begin(); itChild != tdoElement.End(); ++itChild)
      {
        ProcessSequence(*itChild, rScript, rReturnType, rdoResult);
      }
    }

    void ProcessIfeq(const DataObject& rdoContext, const xml::Element& rScript,
                     const DataType& rReturnType, DataObject& rdoResult, bool bEqual)
    {
      if ((Eval(rdoContext, rScript.GetAttributeValue("expr1")) ==
           Eval(rdoContext, rScript.GetAttributeValue("expr2"))) == bEqual)
      {
        ProcessSequence(rdoContext, rScript, rReturnType, rdoResult);
      }
    }


    void ProcessIf(const DataObject& rdoContext, const xml::Element& rScript,
                   const DataType& rReturnType, DataObject& rdoResult)
    {
      bool bProcess = false;

      const std::string& sOp = Eval(rdoContext, rScript.GetAttributeValue("op"));
      const std::string& sValue1 = Eval(rdoContext, rScript.GetAttributeValue("expr1"));
      const std::string& sValue2 = Eval(rdoContext, rScript.GetAttributeValue("expr2"));

      if (sOp == "null")
      {
        bool bNull = false;
        FromString(sValue2, bNull);
        bProcess = (sValue1 == STAFF_DAS_NULL_VALUE) ^ bNull;
      }
      else
      if (sOp == "seq")
      {
        bProcess = sValue1 > sValue2;
      }
      else
      if (sOp == "sgt")
      {
        bProcess = sValue1 > sValue2;
      }
      else
      if (sOp == "slt")
      {
        bProcess = sValue1 < sValue2;
      }
      else
      if (sOp == "sge")
      {
        bProcess = sValue1 >= sValue2;
      }
      else
      if (sOp == "sle")
      {
        bProcess = sValue1 <= sValue2;
      }
      else
      if (sOp == "sne")
      {
        bProcess = sValue1 != sValue2;
      }
      else
      {
        double dValue1 = 0;
        double dValue2 = 0;

        FromString(sValue1, dValue1);
        FromString(sValue2, dValue2);

        if (sOp == "eq")
        {
          bProcess = dValue1 > dValue2;
        }
        else
        if (sOp == "gt")
        {
          bProcess = dValue1 > dValue2;
        }
        else
        if (sOp == "lt")
        {
          bProcess = dValue1 < dValue2;
        }
        else
        if (sOp == "ge")
        {
          bProcess = dValue1 >= dValue2;
        }
        else
        if (sOp == "le")
        {
          bProcess = dValue1 <= dValue2;
        }
        else
        if (sOp == "ne")
        {
          bProcess = dValue1 != dValue2;
        }
        else
        {
          STAFF_THROW_ASSERT("Invalid op: [" + sOp + "]");
        }
      }

      if (bProcess)
      {
        const xml::Element* pThen = rScript.FindChildElementByName("then");
        if (pThen)
        {
          ProcessSequence(rdoContext, *pThen, rReturnType, rdoResult);
        }
      }
      else
      {
        const xml::Element* pElse = rScript.FindChildElementByName("else");
        if (pElse)
        {
          ProcessSequence(rdoContext, *pElse, rReturnType, rdoResult);
        }
      }
    }


    void ProcessSwitch(const DataObject& rdoContext, const xml::Element& rScript,
                       const DataType& rReturnType, DataObject& rdoResult)
    {
      const std::string& sSwitchValue = Eval(rdoContext, rScript.GetAttributeValue("expr"));
      const xml::Element* pElemCase = rScript.GetFirstChildElement();

      for (; pElemCase; pElemCase = pElemCase->GetNextSiblingElement())
      {
        if (pElemCase->GetName() == "default")
        {
          continue;
        }

        if (pElemCase->GetName() != "case")
        {
          LogError() << "Invalid element name: [" << pElemCase->GetName() << "] ignored.";
          continue;
        }

        const std::string& sCaseValue = Eval(rdoContext, pElemCase->GetAttributeValue("expr"));
        if (sSwitchValue == sCaseValue)
        {
          break;
        }
      }

      if (!pElemCase)
      {
        pElemCase = rScript.FindChildElementByName("default");
      }

      if (pElemCase)
      {
        ProcessSequence(rdoContext, *pElemCase, rReturnType, rdoResult);
      }
    }


    void ProcessReturn(const DataObject& rdoContext, const xml::Element& rScript,
                       const DataType& rReturnType, DataObject& rdoResult)
    {
      const xml::Attribute* pVar = rScript.FindAttribute("var");
      if (pVar)
      {
        const std::string& sVarName = pVar->GetValue();
        STAFF_ASSERT(!sVarName.empty(), "variable name is empty");
        if (sVarName[0] == '$')
        {
          const std::string& sValue = Eval(rdoContext, sVarName);
          rdoResult.SetText(sValue);
        }
        else
        {
          VarMap::const_iterator itVar = m_mVars.find(sVarName);
          STAFF_ASSERT(itVar != m_mVars.end(), "Variable [" + sVarName + "] is undefined");
          const Var& rVar = itVar->second;
          STAFF_ASSERT(rVar.tType.eType == rReturnType.eType, "Types mismatch in [return " + sVarName + "]");

          if (rReturnType.eType == DataType::Generic)
          {
            rdoResult.SetText(rVar.sValue);
          }
          else
          { // dataobject, list, struct
            rdoResult = rVar.tdoValue;
          }
        }
      }
      else
      {
        rdoResult.SetText(Eval(rdoContext, rScript.GetValue()));
      }
    }

    // if plsParamsResult == NULL, evaluate string completely
    // if plsParamsResult != NULL, return string with placeholders and put params into plsParamsResult
    std::string Eval(const DataObject& rdoContext, const std::string& sExpr,
                     StringList* plsParamsResult = NULL)
    {
      std::string sResult = sExpr;
      std::string::size_type nBegin = 0;
      std::string::size_type nEnd = 0;
      std::string::size_type nNameBegin = 0;
      std::string::size_type nNameEnd = 0;

      while ((nBegin = sResult.find('$', nEnd)) != std::string::npos)
      {
        if (nBegin > 0 && sResult[nBegin - 1] == '\\')
        {
          sResult.erase(nBegin - 1, 1);
          nEnd = nBegin;
          continue;
        }

        std::string::size_type nSize = sResult.size();
        bool bIsRequest = false;

        STAFF_ASSERT((nBegin + 1) < nSize, "Unexpected '$' at end in [" + sResult + "]");
        if (sResult[nBegin + 1] == '(') // request field
        {
          nEnd = sResult.find(')', nBegin);
          STAFF_ASSERT(nEnd != std::string::npos, "Unexpected end of expression declaration: [" + sResult
                      + "] while processing [" + sResult.substr(nBegin));
          nNameBegin = nBegin + 2;
          nNameEnd = nEnd;
          ++nEnd;
          bIsRequest = true;
        }
        else
        if (sResult[nBegin + 1] == '{') // long variable name
        {
          nEnd = sResult.find('}', nBegin + 2);
          STAFF_ASSERT(nEnd != std::string::npos, "Unexpected end of expression declaration: [" + sResult
                      + "] while processing [" + sResult.substr(nBegin));
          nNameBegin = nBegin + 2;
          nNameEnd = nEnd;
          ++nEnd;
        }
        else
        { // short variable name
          nEnd = sResult.find_first_of(" \n\t\r,.;'\"~`!@#$%^&*()[]{}<>-+=/?", nBegin + 1);

          if (nEnd == std::string::npos)
          {
            nEnd = sResult.size();
          }

          nNameBegin = nBegin + 1;
          nNameEnd = nEnd;
        }

        if (nNameBegin == nNameEnd)
        {
          continue;
        }

        const std::string& sPath = sResult.substr(nNameBegin, nNameEnd - nNameBegin);

        std::string sValue;
        if (bIsRequest)
        { // request
          DataObject tdoResult;
          GetChild(rdoContext, sPath, tdoResult);
          STAFF_ASSERT(!tdoResult.IsNull(), "Node not found while processing eval. NodeName: [" + sPath + "]");
          if (tdoResult.IsNil())
          {
            sValue = STAFF_DAS_NULL_VALUE;
          }
          else
          {
            sValue = tdoResult.GetText();
          }
        }
        else
        { // variable
          // get var name
          std::string sVarName;
          std::string::size_type nPos = sPath.find('.');
          if (nPos != std::string::npos)
          {
            sVarName = sPath.substr(0, nPos);
          }
          else
          {
            sVarName = sPath;
          }

          STAFF_ASSERT(!sVarName.empty(), "Invalid var name: [" + sPath + "]");

          if (sVarName == "nodeValue")
          {
            sValue = rdoContext.GetText();
          }
          else
          if (sVarName == "nodeName")
          {
            sValue = rdoContext.GetLocalName();
          }
          else
          {
            VarMap::const_iterator itVar = m_mVars.find(sVarName);
            STAFF_ASSERT(itVar != m_mVars.end(), "Variable [" + sVarName + "] is undefined");

            const Var& rVar = itVar->second;
            STAFF_ASSERT(rVar.tType.eType != DataType::Void, "found void type variable [" + sVarName
                         + "] while evaluating expression [" + sExpr + "]");

            if (rVar.tType.eType == DataType::Generic)
            {
              sValue = rVar.sValue;
            }
            else
            { // dataobject, list, struct
              if (nPos != std::string::npos)
              {
                DataObject tdoResult;
                GetChild(rVar.tdoValue, sPath.substr(nPos + 1), tdoResult);
                STAFF_ASSERT(!tdoResult.IsNull(), "Node not found while processing eval. NodeName: [" + sPath + "]");
                if (tdoResult.IsNil())
                {
                  sValue = STAFF_DAS_NULL_VALUE;
                }
                else
                {
                  sValue = tdoResult.GetText();
                }
              }
              else
              {
                if (rVar.tdoValue.IsNil())
                {
                  sValue = STAFF_DAS_NULL_VALUE;
                }
                else
                {
                  sValue = rVar.tdoValue.GetText();
                }
              }
            }
          }
        } // if request

        if (plsParamsResult)
        {
          plsParamsResult->push_back(sValue);
          sResult.replace(nBegin, nEnd - nBegin, "?");
          nEnd = nBegin + 1;
        }
        else
        {
          sResult.replace(nBegin, nEnd - nBegin, sValue);
          nEnd = nBegin + sValue.size();
        }
      } // while find '$'

      return sResult;
    }

    void ProcessLog(const DataObject& rdoContext, const xml::Element& rScript)
    {
      LogInfo() << m_rDataSource.GetName() << ": " << Eval(rdoContext, rScript.GetValue());
    }

    void ProcessFault(const DataObject& rdoContext, const xml::Element& rScript)
    {
      STAFF_THROW_ASSERT("Error while invoking Operation [" + rdoContext.GetLocalName() +
                  "] in datasource [" + m_rDataSource.GetName() +
                  "]: " + Eval(rdoContext, rScript.GetValue()));
    }

    void ProcessTryCatch(const DataObject& rdoContext, const xml::Element& rScript,
                         const DataType& rReturnType, DataObject& rdoResult)
    {
      const xml::Element& rTry = rScript.GetChildElementByName("try");
      const xml::Element& rCatch = rScript.GetChildElementByName("catch");

      try
      {
        ProcessSequence(rdoContext, rTry, rReturnType, rdoResult);
      }
      catch (const std::exception& rEx)
      {
        const xml::Attribute* pAttrVar = rCatch.FindAttribute("var");
        if (pAttrVar)
        {
          Var& rVar = m_mVars[pAttrVar->GetValue()];
          rVar.tType.eType = DataType::Generic;
          rVar.sValue = rEx.what();
        }
        ProcessSequence(rdoContext, rCatch, rReturnType, rdoResult);
      }
    }

    void CreateDataObject(DataObject& rdoContext, const std::string& sName,
                          const std::string& sChildPath, DataObject& rdoResult)
    {
      if (!rdoContext.IsInit())
      {
        rdoContext.Create(sName);
      }
      GetChild(rdoContext, sChildPath, rdoResult, true);
    }

    void GetChild(const DataObject& rdoContext, const std::string& sChildPath, DataObject& rdoResult,
                  bool bCreate = false)
    {
      rdoResult = rdoContext.Copy();

      if (!sChildPath.empty())
      {
        std::string::size_type nSize = sChildPath.size();
        STAFF_ASSERT(nSize > 1, "invalid node name: [" + sChildPath + "]");

        std::string sPath;
        if (sChildPath[0] == '$')
        {
          if (sChildPath[1] == '[' && sChildPath[nSize - 1] == ']')
          {
            sPath = sChildPath.substr(2, nSize - 3);
            std::string::size_type nPos = sPath.find('.');
            if (nPos == std::string::npos)
            {
              rdoResult = m_mVars[sPath].tdoValue.Copy();
              return;
            }
            else
            {
              rdoResult = m_mVars[sPath.substr(0, nPos)].tdoValue.Copy();
              sPath.erase(0, nPos + 1);
            }
          }
          else
          if (sChildPath[1] == '(' && sChildPath[nSize - 1] == ')')
          {
            sPath = sChildPath.substr(2, nSize - 3);
          }
          else
          {
            LogError() << "Invalid path: [" << sChildPath << "]";
            return;
          }
        }
        else
        {
          sPath = sChildPath;
        }


        std::string::size_type nBegin = 0;
        std::string::size_type nEnd = 0;
        std::string sName;
        do
        {
          nEnd = sPath.find('.', nBegin);
          if (nEnd == std::string::npos)
          {
            sName = sPath.substr(nBegin);
          }
          else
          {
            sName = sPath.substr(nBegin, nEnd - nBegin);
          }

          if (sName.empty()) // parent node
          {
            rdoResult = rdoResult.Parent();
            STAFF_ASSERT(!rdoResult.IsNull(), "parent of root element reached while GetChild[" + sChildPath + "]");
          }
          else
          {
            if (!bCreate)
            {
              rdoResult = rdoResult.GetChildByLocalName(sName);
            }
            else
            {
              rdoResult = rdoResult.CreateChildOnce(sName);
            }
          }

          nBegin = nEnd + 1;
        }
        while(nEnd != std::string::npos);
      }
    }

  public:
    const DataSource& m_rDataSource;
    Providers& m_rstProviders;
    VarMap m_mVars;
    ExecutorsMap m_mExec;
    bool m_bReturn;
  };


  ScriptExecuter::ScriptExecuter(const DataSource& rDataSource, Providers& rstProviders)
  {
    m_pImpl = new ScriptExecuterImpl(rDataSource, rstProviders);
  }

  ScriptExecuter::~ScriptExecuter()
  {
    delete m_pImpl;
  }

  void ScriptExecuter::Process(const DataObject& rdoOperation, DataObject& rdoResult)
  {
    const Operation* pOperation = &m_pImpl->m_rDataSource.GetOperation(rdoOperation.GetLocalName());

    m_pImpl->ProcessSequence(rdoOperation, pOperation->tScript, pOperation->stReturn, rdoResult);

    if (pOperation->stReturn.eType == DataType::Generic)
    {
      STAFF_ASSERT(!rdoResult.IsTextNull(), "Empty result for generic type!");
    }
  }

  void ScriptExecuter::Process(const DataObject& rdoContext, const xml::Element& rScript,
                               const DataType& rReturnType, DataObject& rdoResult)
  {
    m_pImpl->ProcessSequence(rdoContext, rScript, rReturnType, rdoResult);
    if (rReturnType.eType == DataType::Generic)
    {
      STAFF_ASSERT(!rdoResult.IsTextNull(), "Empty result for generic type!");
    }
  }

  void ScriptExecuter::Process(const xml::Element& rScript)
  {
    staff::DataObject doResult;
    m_pImpl->ProcessSequence(staff::DataObject(), rScript, DataType(), doResult);
  }


} // namespace das
} // namespace staff
