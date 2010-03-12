/*
 *  Copyright 2009 Utkin Dmitry
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

// This file generated by staff_codegen
// Service Implementation

#include <unistd.h>
#include <rise/common/Log.h>
#include <rise/xml/XMLNode.h>
#include <rise/string/String.h>
#include <rise/string/Encoding.h>
#include <rise/xml/XMLException.h>
#include <rise/tools/FileFind.h>
#include <staff/common/Exception.h>
#include <staff/common/Runtime.h>
#include <staff/common/DataObject.h>
#include <staff/common/DataObjectHelper.h>
#include <staff/security/Sessions.h>
#include <staff/security/Objects.h>
#include <staff/security/UsersToGroups.h>
#include <staff/security/Acl.h>
#include "WidgetManagerContext.h"
#include "WidgetManagerImpl.h"

namespace widget
{

  struct CWidgetManagerImpl::SWidgetInternal
  {
    std::string sId;
    std::string sClass;
    rise::xml::CXMLNode tProps;

    void ToWidget(SWidget& rWidget) const
    {
      rWidget.sId = sId;
      rWidget.sClass = sClass;
      staff::CDataObjectHelper::XmlToDataObject(tProps, rWidget.tdoProps);
    }

    void FromWidget(const SWidget& rWidget)
    {
      sId = rWidget.sId;
      sClass = rWidget.sClass;
      tProps.Clear();
      staff::CDataObjectHelper::DataObjectToXml(rWidget.tdoProps, tProps);
    }
  };

  struct CWidgetManagerImpl::SWidgetGroupInternal
  {
    std::string         sId;
    std::string         sDescr;
    TWidgetInternalMap  mWidgets;

    void ToWidgetGroup(SWidgetGroup& rWidgetGroup) const
    {
      rWidgetGroup.sId = sId;
      rWidgetGroup.sDescr = sDescr;
      rWidgetGroup.mWidgets.clear();
      
      for (TWidgetInternalMap::const_iterator itWidget = mWidgets.begin();
           itWidget != mWidgets.end(); ++itWidget)
      {
        itWidget->second.ToWidget(rWidgetGroup.mWidgets[itWidget->first]);
      }
    }

    void FromWidgetGroup(const SWidgetGroup& rWidgetGroup)
    {
      sId = rWidgetGroup.sId;
      sDescr = rWidgetGroup.sDescr;
      mWidgets.clear();

      for (TWidgetMap::const_iterator itWidget = rWidgetGroup.mWidgets.begin();
        itWidget != rWidgetGroup.mWidgets.end(); ++itWidget)
      {
        mWidgets[itWidget->first].FromWidget(itWidget->second);
      }
    }
  };

  CWidgetManagerImpl::CWidgetManagerImpl():
    m_nUserId(-1), m_nIsUserAdmin(-1)
  {
  }

  CWidgetManagerImpl::~CWidgetManagerImpl()
  {
  }

  TBaseProfileList CWidgetManagerImpl::GetBaseProfiles()
  {
    TBaseProfileList tResult;

    rise::xml::CXMLDocument tDoc;

    tDoc.LoadFromFile(m_sDbPath + "baseprofiles.xml");

    bool bIsUserAdmin = IsUserAdmin();

    const rise::xml::CXMLNode& rNodeRoot = tDoc.GetRoot();

    for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeProfile = rNodeRoot.NodeBegin();
        itNodeProfile != rNodeRoot.NodeEnd(); ++itNodeProfile)
    {
      const rise::xml::CXMLNode& rNodeProfile = *itNodeProfile;
      if (rNodeProfile.NodeType() == rise::xml::CXMLNode::ENTGENERIC && rNodeProfile.NodeName() == "Profile")
      {
        SBaseProfile stProfile;
        stProfile.sId = rNodeProfile["Id"].AsString();
        stProfile.sName = rNodeProfile["Name"].AsString();

        bool bIsAdminProfile = rNodeProfile["IsAdmin"].AsString() != "false" &&
                               rNodeProfile["IsAdmin"].AsString() != "0";

        if ((!bIsAdminProfile) || (bIsAdminProfile && bIsUserAdmin))
        {
          tResult.push_back(stProfile);
        }
      }
    }

    return tResult;  // result
  }

  void CWidgetManagerImpl::LoadProfiles(const std::string& sFile, TProfileList& rlsProfiles)
  {
    rise::xml::CXMLDocument tDoc;
    tDoc.LoadFromFile(sFile);

    rise::xml::CXMLNode& rNodeRoot = tDoc.GetRoot();
    for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeProfile = rNodeRoot.NodeBegin();
        itNodeProfile != rNodeRoot.NodeEnd(); ++itNodeProfile)
    {
      const rise::xml::CXMLNode& rNodeProfile = *itNodeProfile;
      if (rNodeProfile.NodeType() == rise::xml::CXMLNode::ENTGENERIC && rNodeProfile.NodeName() == "Profile")
      {
        SProfile stProfile;
        stProfile.sId = rNodeProfile["Id"].AsString();
        stProfile.sName = rNodeProfile["Name"].AsString();
        stProfile.sBase = rNodeProfile["Base"].AsString();
        rlsProfiles.push_back(stProfile);
      }
    }
  }

  TProfileList CWidgetManagerImpl::GetProfiles()
  {
    TProfileList tResult;

    const std::string& sUserProfilesPath = m_sDbPath + "profiles/";

    try
    {
      const std::string& sUserProfilesFileName = sUserProfilesPath + rise::ToStr(GetUserId()) + ".xml";
      LoadProfiles(sUserProfilesFileName, tResult);
    }
    catch(...)
    {
      const std::string& sUserProfilesFileName = sUserProfilesPath +
        (IsUserAdmin() ? "admin.default.xml" : "user.default.xml");

      rise::LogWarning() << "using default profile list: " << sUserProfilesFileName;

      try
      {
        LoadProfiles(sUserProfilesFileName, tResult);
      }
      catch(...)
      {
        rise::LogError() << "Can't load default profile list";
      }
    }

    return tResult;  // result
  }

  void CWidgetManagerImpl::AddProfile(const SProfile& stProfile)
  {
    {
      const std::string& sWidgetsPath = m_sDbPath + "widgets/";
      const std::string& sUserDbFileName = sWidgetsPath + stProfile.sId + "." + rise::ToStr(GetUserId()) + ".xml";
      const std::string& sBaseDbFileName = sWidgetsPath + stProfile.sBase + ".default.xml";

      rise::xml::CXMLDocument tDoc;

      try
      {
        rise::LogDebug() << "Loading " << sBaseDbFileName;
        tDoc.LoadFromFile(sBaseDbFileName);
      }
      catch(...)
      {
        rise::LogWarning() << "Using DEFAULT DB as base for profile " << stProfile.sId << " : "
            << sWidgetsPath << "default.xml";
        tDoc.LoadFromFile(sWidgetsPath + "default.xml");
      }

      rise::LogDebug() << "Saving " << sUserDbFileName;
      tDoc.SaveToFile(sUserDbFileName);
    }

    // changing profile list
    {
      const std::string& sUserProfilesPath = m_sDbPath + "profiles/";
      const std::string& sUserProfilesFileName = sUserProfilesPath + rise::ToStr(GetUserId()) + ".xml";

      rise::xml::CXMLDocument tDoc;
      rise::xml::CXMLNode& rNodeRoot = tDoc.GetRoot();

      try
      {
        rise::LogDebug2() << "Loading " << sUserProfilesFileName;
        tDoc.LoadFromFile(sUserProfilesFileName);
      }
      catch(...)
      {
        const std::string& sUserProfilesFileName = sUserProfilesPath +
          (IsUserAdmin() ? "admin.default.xml" : "user.default.xml");

        rise::LogWarning() << "loading default profile list: " << sUserProfilesFileName;

        try
        {
          rise::LogDebug2() << "Loading " << sUserProfilesFileName;
          tDoc.LoadFromFile(sUserProfilesFileName);
        }
        catch(...)
        {
          rise::LogWarning() << "can't load: " << sUserProfilesFileName
              << ". creating empty profile list";
          rNodeRoot.NodeName() = "ProfileList";
        }
      }

      rise::xml::CXMLNode& rNodeProfile = rNodeRoot.AddSubNode("Profile");
      rNodeProfile.AddSubNode("Id").NodeContent() = stProfile.sId;
      rNodeProfile.AddSubNode("Name").NodeContent() = stProfile.sName;
      rNodeProfile.AddSubNode("Base").NodeContent() = stProfile.sBase;

      rise::LogDebug() << "Saving as: " << sUserProfilesFileName;
      tDoc.SaveToFile(sUserProfilesFileName);
    }
  }

  void CWidgetManagerImpl::DeleteProfile(const std::string& sProfile)
  {
    const std::string& sProfilesListFileName = m_sDbPath + "profiles/" + rise::ToStr(GetUserId()) + ".xml";

    rise::xml::CXMLDocument tDoc;
    rise::xml::CXMLNode& rNodeRoot = tDoc.GetRoot();

    try
    {
      tDoc.LoadFromFile(sProfilesListFileName);
    }
    catch(...)
    {
      return;
    }

    for (rise::xml::CXMLNode::TXMLNodeIterator itNodeProfile = rNodeRoot.NodeBegin();
        itNodeProfile != rNodeRoot.NodeEnd(); ++itNodeProfile)
    {
      const rise::xml::CXMLNode& rNodeProfile = *itNodeProfile;
      if (rNodeProfile["Id"] == sProfile)
      {
        rNodeRoot.DelSubNode(itNodeProfile);
        tDoc.SaveToFile(sProfilesListFileName);
        break;
      }
    }

    const std::string& sProfileFile = m_sDbPath + "widgets/" + sProfile + "." + rise::ToStr(GetUserId()) + ".xml";
    unlink(sProfileFile.c_str());
  }

  void CWidgetManagerImpl::SetProfile(const SProfile& stProfile)
  {
    // changing profile list
    {
      const std::string& sProfilesListFileName = m_sDbPath + "profiles/" + rise::ToStr(GetUserId()) + ".xml";

      rise::xml::CXMLDocument tDoc;
      rise::xml::CXMLNode& rNodeRoot = tDoc.GetRoot();

      rise::LogDebug() << "Loading " << sProfilesListFileName;
      tDoc.LoadFromFile(sProfilesListFileName);

      for (rise::xml::CXMLNode::TXMLNodeIterator itNodeProfile = rNodeRoot.NodeBegin();
          itNodeProfile != rNodeRoot.NodeEnd(); ++itNodeProfile)
      {
        rise::xml::CXMLNode& rNodeProfile = *itNodeProfile;
        if (rNodeProfile["Id"] == stProfile.sId)
        {
          rNodeProfile["Name"] = stProfile.sName;
          rise::LogDebug1() << "Saving " << sProfilesListFileName;
          tDoc.SaveToFile(sProfilesListFileName);
          break;
        }
      }
    }
  }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  // widgets

  void CWidgetManagerImpl::Open(const std::string& sProfile)
  {
    Close();

    std::string sBaseProfile;
//    std::string sProfileName;

    // get profile base
    {
      const std::string& sUserProfilesFileName = m_sDbPath + "profiles/" + rise::ToStr(GetUserId()) + ".xml";

      rise::xml::CXMLDocument tDocProfiles;
      rise::xml::CXMLNode& rNodeProfilesRoot = tDocProfiles.GetRoot();

      rise::LogDebug2() << "Loading " << sUserProfilesFileName;

      try
      {
        tDocProfiles.LoadFromFile(sUserProfilesFileName);
      }
      catch(...)
      {
        const std::string& sUserProfilesFileName = m_sDbPath + "profiles/" +
          (IsUserAdmin() ? "admin.default.xml" : "user.default.xml");

        rise::LogWarning() << "using default profile list: " << sUserProfilesFileName;

        tDocProfiles.LoadFromFile(sUserProfilesFileName);
      }

      for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeProfile = rNodeProfilesRoot.NodeBegin();
          itNodeProfile != rNodeProfilesRoot.NodeEnd(); ++itNodeProfile)
      {
        const rise::xml::CXMLNode& rNodeProfile = *itNodeProfile;
        if (rNodeProfile.NodeType() == rise::xml::CXMLNode::ENTGENERIC &&
            rNodeProfile.NodeName() == "Profile" &&
            rNodeProfile["Id"].AsString() == sProfile)
        {
          sBaseProfile = rNodeProfile["Base"].AsString();
//          sProfileName = rNodeProfile["Name"].AsString();
        }
      }
    }

    RISE_ASSERTES(sBaseProfile.size() != 0, rise::CLogicNoItemException, "can't get profile " + sProfile);


    // loading widgets list
    const std::string& sWidgetsPath = m_sDbPath + "widgets/";
    const std::string& sUserDbFileName = sWidgetsPath + sProfile + "." + rise::ToStr(GetUserId()) + ".xml";
    const rise::xml::CXMLNode& rNodeRoot = m_tDoc.GetRoot();

    try
    {
      m_tDoc.LoadFromFile(sUserDbFileName);
    }
    catch(...)
    {
      try
      {
        const std::string& sUserDbFileName = sWidgetsPath + sBaseProfile + ".default.xml";
        rise::LogWarning() << "Loading DEFAULT DB for profile: " << sUserDbFileName;
        m_tDoc.LoadFromFile(sUserDbFileName);
      }
      catch(...)
      {
        const std::string& sUserDbFileName = sWidgetsPath + "default.xml";
        rise::LogWarning() << "Loading DEFAULT DB: " << sUserDbFileName;
        m_tDoc.LoadFromFile(sUserDbFileName);
      }

      rise::LogWarning() << "Widget DB will be created in " + sUserDbFileName;
    }

    RISE_ASSERTES(rNodeRoot.Attribute("version") == "2.0", rise::CFileOpenException, "Widget DB version mismatch");

    LoadProfileWidgetList(sBaseProfile);

    // Load widgets
    LoadWidgets(rNodeRoot.Subnode("Widgets"), m_mActiveWidgets);

    // Load Available groups
    m_mWidgetGroups.clear();
    const rise::xml::CXMLNode& rNodeWidgetGroups = rNodeRoot.Subnode("WidgetGroups");
    for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeWidgetGroup = rNodeWidgetGroups.NodeBegin(); 
      itNodeWidgetGroup != rNodeWidgetGroups.NodeEnd(); ++itNodeWidgetGroup)
    {
      if ((itNodeWidgetGroup->NodeType() == rise::xml::CXMLNode::ENTGENERIC) && 
          (itNodeWidgetGroup->NodeName() == "WidgetGroup"))
      {
        const rise::xml::CXMLNode& rNodeWidgetGroup = *itNodeWidgetGroup;
        const std::string& sWidgetGroupId = rNodeWidgetGroup["sId"].AsString();
        const std::string& sDescr = rNodeWidgetGroup["sDescr"].AsString();
        
        SWidgetGroupInternal& rWidgetGroup = m_mWidgetGroups[sWidgetGroupId];

        rWidgetGroup.sId = sWidgetGroupId;
        rWidgetGroup.sDescr = sDescr;

        LoadWidgets(rNodeWidgetGroup.Subnode("Widgets"), rWidgetGroup.mWidgets);
      }
    }

    // Load Active Groups
    m_lsActiveWidgetGroups.clear();
    const rise::xml::CXMLNode& rNodeActiveWidgetGroups = rNodeRoot.Subnode("ActiveWidgetGroups");
    for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeActiveWidgetGroup = rNodeActiveWidgetGroups.NodeBegin(); 
      itNodeActiveWidgetGroup != rNodeActiveWidgetGroups.NodeEnd(); ++itNodeActiveWidgetGroup)
    {
      if ((itNodeActiveWidgetGroup->NodeType() == rise::xml::CXMLNode::ENTGENERIC) && 
          (itNodeActiveWidgetGroup->NodeName() == "WidgetGroup"))
      {
        m_lsActiveWidgetGroups.push_back(itNodeActiveWidgetGroup->NodeContent());
      }
    }

    m_sUserDbFileName = sUserDbFileName;
    m_sProfile = sProfile;
  }
  
  void CWidgetManagerImpl::LoadWidgets( const rise::xml::CXMLNode& rNodeWidgets, TWidgetInternalMap& mWidgets )
  {
    mWidgets.clear();

    for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeWidgets = rNodeWidgets.NodeBegin(); 
          itNodeWidgets != rNodeWidgets.NodeEnd(); ++itNodeWidgets)
    {
      if ((itNodeWidgets->NodeType() == rise::xml::CXMLNode::ENTGENERIC) && 
          (itNodeWidgets->NodeName() == "Widget"))
      {
        const rise::xml::CXMLNode& rNodeWidget = *itNodeWidgets;
        const std::string& sWidgetClassName = rNodeWidget["sClass"].AsString();

        // filter-out unknown classes
        TStringMap::const_iterator itClass = m_mWidgetClasses.find(sWidgetClassName);
        if(itClass != m_mWidgetClasses.end())
        {
          const rise::xml::CXMLNode& rNodeProperties = rNodeWidget.Subnode("Properties");
          const std::string& sWidgetId = rNodeWidget["sId"].AsString();

          SWidgetInternal& stWidget = mWidgets[sWidgetId];

          stWidget.sClass = sWidgetClassName;
          stWidget.sId = sWidgetId;
          stWidget.tProps = rNodeProperties;
        }
        else
        {
          rise::LogWarning() << "Class \'" << sWidgetClassName << "\'is not listed in classdb. Ignoring.";
        }
      }
    } // for
  }

  void CWidgetManagerImpl::Close()
  {
    if (m_sUserDbFileName.size() == 0)
    {
      return;
    }

    try
    {
      Commit();
    }
    RISE_CATCH_ALL

    m_sUserDbFileName.erase();
  }

  void CWidgetManagerImpl::Commit()
  {
    RISE_ASSERTES(m_sUserDbFileName != "", staff::CRemoteException, "DB was not opened");

    rise::xml::CXMLNode& rNodeRoot = m_tDoc.GetRoot();

    // widgets
    {
      rise::xml::CXMLNode& rNodeWidgets = rNodeRoot.Subnode("Widgets");
      rNodeWidgets.Clear();
      for(TWidgetInternalMap::const_iterator itWidget = m_mActiveWidgets.begin(); 
        itWidget != m_mActiveWidgets.end(); ++itWidget)
      {
        const SWidgetInternal& rWidgetInternal = itWidget->second;
        rise::xml::CXMLNode& rNodeWidget = rNodeWidgets.AddSubNode("Widget");
        rNodeWidget.AddSubNode("sClass").NodeContent().AsString() = rWidgetInternal.sClass;
        rNodeWidget.AddSubNode("sId").NodeContent().AsString() = rWidgetInternal.sId;
        rNodeWidget.AddSubNode("Properties") = rWidgetInternal.tProps;
      }
    }

    // widget groups
    {
      rise::xml::CXMLNode& rNodeWidgetGroups = rNodeRoot.Subnode("WidgetGroups");
      rNodeWidgetGroups.Clear();
      for(TWidgetGroupInternalMap::const_iterator itWidgetGroup = m_mWidgetGroups.begin(); 
        itWidgetGroup != m_mWidgetGroups.end(); ++itWidgetGroup)
      {
        const SWidgetGroupInternal& rWidgetGroupInternal = itWidgetGroup->second;
        rise::xml::CXMLNode& rNodeWidgetGroup = rNodeWidgetGroups.AddSubNode("WidgetGroup");
        rNodeWidgetGroup.AddSubNode("sId").NodeContent().AsString() = rWidgetGroupInternal.sId;
        rNodeWidgetGroup.AddSubNode("sDescr").NodeContent().AsString() = rWidgetGroupInternal.sDescr;

        rise::xml::CXMLNode& rNodeWidgets = rNodeWidgetGroup.AddSubNode("Widgets");

        for(TWidgetInternalMap::const_iterator itWidget = rWidgetGroupInternal.mWidgets.begin(); 
          itWidget != rWidgetGroupInternal.mWidgets.end(); ++itWidget)
        {
          const SWidgetInternal& rWidgetInternal = itWidget->second;
          rise::xml::CXMLNode& rNodeWidget = rNodeWidgets.AddSubNode("Widget");
          rNodeWidget.AddSubNode("sClass").NodeContent().AsString() = rWidgetInternal.sClass;
          rNodeWidget.AddSubNode("sId").NodeContent().AsString() = rWidgetInternal.sId;
          rNodeWidget.AddSubNode("Properties") = rWidgetInternal.tProps;
        }
      }
    }

    // active groups
    {
      rise::xml::CXMLNode& rNodeActiveWidgetGroups = rNodeRoot.Subnode("ActiveWidgetGroups");
      rNodeActiveWidgetGroups.Clear();

      for (TStringList::const_iterator itWidgetGroup = m_lsActiveWidgetGroups.begin(); 
        itWidgetGroup != m_lsActiveWidgetGroups.end(); ++itWidgetGroup)
      {
        rNodeActiveWidgetGroups.AddSubNode("WidgetGroup").NodeContent() = *itWidgetGroup;
      }
    }

    m_tDoc.SaveToFile(m_sUserDbFileName);
  }


  TStringMap CWidgetManagerImpl::GetWidgetClasses()
  {
    return m_mWidgetClasses;
  }


  TWidgetMap CWidgetManagerImpl::GetActiveWidgets() const
  {
    TWidgetMap mWidgets;

    for (TWidgetInternalMap::const_iterator itActiveWidget = m_mActiveWidgets.begin();
      itActiveWidget != m_mActiveWidgets.end(); ++itActiveWidget)
    {
      itActiveWidget->second.ToWidget(mWidgets[itActiveWidget->first]);
    }

    return mWidgets;
  }

  void CWidgetManagerImpl::AddWidget( const SWidget& rWidget )
  {
    RISE_ASSERTES(m_mActiveWidgets.find(rWidget.sId) == m_mActiveWidgets.end(), 
      staff::CRemoteException, "Widget with id \'" + rWidget.sId + "\'already exists");

    m_mActiveWidgets[rWidget.sId].FromWidget(rWidget);
  }

  void CWidgetManagerImpl::AddWidgets(const TWidgetMap& rWidgets)
  {
    for (TWidgetMap::const_iterator itWidget = rWidgets.begin();
      itWidget != rWidgets.end(); ++itWidget)
    {
      RISE_ASSERTES(m_mActiveWidgets.find(itWidget->first) == m_mActiveWidgets.end(),
        staff::CRemoteException, "Widget with id \'" + itWidget->first + "\'already exists");

      m_mActiveWidgets[itWidget->first].FromWidget(itWidget->second);
    }
  }
  
  void CWidgetManagerImpl::DeleteWidget( const std::string& sId )
  {
    m_mActiveWidgets.erase(sId);
  }

  void CWidgetManagerImpl::DeleteWidgets( const TStringList& lsIds )
  {
    for (TStringList::const_iterator itWidgetId = lsIds.begin(); 
      itWidgetId != lsIds.end(); ++itWidgetId)
    {
      m_mActiveWidgets.erase(*itWidgetId);
    }
  }

  void CWidgetManagerImpl::AlterWidget( const SWidget& rWidget )
  {
    TWidgetInternalMap::iterator itWidget = m_mActiveWidgets.find(rWidget.sId);
    RISE_ASSERTES(itWidget != m_mActiveWidgets.end(), staff::CRemoteException, "Widget does not exists: " + rWidget.sId);

    itWidget->second.FromWidget(rWidget);
  }

  void CWidgetManagerImpl::AlterWidgetsListAndCommit(const TWidgetList& rlsWidgets)
  {
    for (TWidgetList::const_iterator itWidget = rlsWidgets.begin();
         itWidget != rlsWidgets.end(); ++itWidget)
    {
      AlterWidget(*itWidget);
    }

    Commit();
  }


  TStringMap CWidgetManagerImpl::GetAvailableWidgetGroups() const
  {
    TStringMap mResult;
    for (TWidgetGroupInternalMap::const_iterator itWidgetGroup = m_mWidgetGroups.begin();
          itWidgetGroup != m_mWidgetGroups.end(); ++itWidgetGroup)
    {
      mResult[itWidgetGroup->second.sId] = itWidgetGroup->second.sDescr;
    }
    
    return mResult;
  }

  TWidgetGroupMap CWidgetManagerImpl::GetWidgetGroups(const TStringList& lsWidgetGroups) const
  {
    TWidgetGroupMap mWidgetGroups;
    for (TStringList::const_iterator itWidgetGroup = lsWidgetGroups.begin();
        itWidgetGroup != lsWidgetGroups.end(); ++itWidgetGroup)
    {
      TWidgetGroupInternalMap::const_iterator itFindWidgetGroup = m_mWidgetGroups.find(*itWidgetGroup);
      if (itFindWidgetGroup != m_mWidgetGroups.end())
      {
        itFindWidgetGroup->second.ToWidgetGroup(mWidgetGroups[itFindWidgetGroup->first]);
      }
    }
    
    return mWidgetGroups;
  }


  void CWidgetManagerImpl::AddWidgetGroup( const SWidgetGroup& rWidgetGroup )
  {
    RISE_ASSERTES(m_mWidgetGroups.find(rWidgetGroup.sId) == m_mWidgetGroups.end(), 
      staff::CRemoteException, "Widget group with id: \'" + rWidgetGroup.sId + "\' already exists");
    m_mWidgetGroups[rWidgetGroup.sId].FromWidgetGroup(rWidgetGroup);
  }

  void CWidgetManagerImpl::DeleteWidgetGroup( const std::string& sGroupId )
  {
    m_mWidgetGroups.erase(sGroupId);
  }

  void CWidgetManagerImpl::AlterWidgetGroup( const SWidgetGroup& rWidgetGroup )
  {
    TWidgetGroupInternalMap::iterator itWidgetGroup = m_mWidgetGroups.find(rWidgetGroup.sId);
    RISE_ASSERTES(itWidgetGroup != m_mWidgetGroups.end(),
      staff::CRemoteException, "Widget group with id: \'" + rWidgetGroup.sId + "\' is not found");
    itWidgetGroup->second.FromWidgetGroup(rWidgetGroup);
  }

  TStringList CWidgetManagerImpl::GetActiveWidgetGroups() const
  {
    return m_lsActiveWidgetGroups;
  }

  void CWidgetManagerImpl::SetActiveWidgetGroups( const TStringList& lsActiveWidgetGroups )
  {
    m_lsActiveWidgetGroups = lsActiveWidgetGroups;
  }


  const std::string& CWidgetManagerImpl::GetSessionId()
  {
    if(m_sSessionId.size() == 0)
    {
      m_sSessionId = CWidgetManagerContext::GetContext().GetServiceID(this);
    }

    return m_sSessionId;
  }

  int CWidgetManagerImpl::GetUserId()
  {
    if (m_nUserId == -1)
    {
      staff::security::CSessions::Inst().GetUserId(GetSessionId(), m_nUserId);
    }

    return m_nUserId;
  }

  bool CWidgetManagerImpl::IsUserAdmin()
  {
    if (m_nIsUserAdmin == -1)
    {
      m_nIsUserAdmin = staff::security::CUsersToGroups::Inst().IsUserMemberOfGroup(GetUserId(), 0) ? 1 : 0;
    }
    return m_nIsUserAdmin == 1;
  }

  void CWidgetManagerImpl::LoadWidgetsNames()
  {
    m_mWidgetsNames.clear();

    rise::xml::CXMLDocument tDoc;

    tDoc.LoadFromFile(m_sDbPath + "widgets.xml");

    const rise::xml::CXMLNode& rNodeRoot = tDoc.GetRoot();

    for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeWidget = rNodeRoot.NodeBegin();
        itNodeWidget != rNodeRoot.NodeEnd(); ++itNodeWidget)
    {
      const rise::xml::CXMLNode& rNodeWidget = *itNodeWidget;
      if (rNodeWidget.NodeType() == rise::xml::CXMLNode::ENTGENERIC &&
          rNodeWidget.NodeName() == "Widget")
      {
        m_mWidgetsNames[rNodeWidget["Class"].AsString()] = rNodeWidget["Name"].AsString();
      }
    }
  }

  void CWidgetManagerImpl::LoadProfileWidgetList( const std::string& sBaseProfile )
  {
    // TODO: load class db only once per profile
    // TODO: check file for update, and reload only when file date is changed
//     if (m_sProfile == sProfile)
//     {
//       return m_mWidgetClasses;
//     }

    LoadWidgetsNames();
    
    // widget classes
    m_mWidgetClasses.clear();

    std::string sBaseProfilesFileName = m_sDbPath + "baseprofiles.xml";

    rise::xml::CXMLDocument tDocBaseProfiles;
    tDocBaseProfiles.LoadFromFile(sBaseProfilesFileName);

    const rise::xml::CXMLNode& rNodeRoot = tDocBaseProfiles.GetRoot();
    RISE_ASSERTES(rNodeRoot.Attribute("version") == "1.0", rise::CFileOpenException, "Widget baseprofiles DB version mismatch");

    const rise::xml::CXMLNode* pNodeWidgets = NULL;

    for (rise::xml::CXMLNode::TXMLNodeConstIterator itNodeProfile = rNodeRoot.NodeBegin();
        itNodeProfile != rNodeRoot.NodeEnd(); ++itNodeProfile)
    {
      const rise::xml::CXMLNode& rNodeProfile = *itNodeProfile;
      if (rNodeProfile.NodeType() == rise::xml::CXMLNode::ENTGENERIC &&
          rNodeProfile.NodeName() == "Profile" &&
          rNodeProfile["Id"].AsString() == sBaseProfile)
      {
        pNodeWidgets = &rNodeProfile.Subnode("Widgets");
        break;
      }
    }

    RISE_ASSERTES(pNodeWidgets, rise::CLogicNoItemException, "can't find base profile: " + sBaseProfile);


    staff::security::CAcl& rAcl = staff::security::CAcl::Inst();

    for(rise::xml::CXMLNode::TXMLNodeConstIterator itWidget = pNodeWidgets->NodeBegin();
        itWidget != pNodeWidgets->NodeEnd(); ++itWidget)
    {
      if ((itWidget->NodeType() == rise::xml::CXMLNode::ENTGENERIC) &&
          (itWidget->NodeName() == "Widget"))
      {
        const std::string& sWidgetClass = itWidget->NodeContent();

        TStringMap::const_iterator itWidgetName = m_mWidgetsNames.find(sWidgetClass);
        if (itWidgetName == m_mWidgetsNames.end())
        {
          rise::LogWarning() << "can't get widget name for class: " << sWidgetClass << ". skipping this widget.";
          continue;
        }

        const std::string& sWidgetName = itWidgetName->second;

        if (rAcl.CalculateUserAccess(sWidgetName, m_nUserId))
        {
          rise::LogDebug1() << "adding widget " << sWidgetClass << "(" << sWidgetName << ") to user " << m_nUserId;
          m_mWidgetClasses[sWidgetClass] = sWidgetName;
        }
        else
        {
          rise::LogDebug1() << "skipping widget " << sWidgetClass << "(" << sWidgetName << ") to user " << m_nUserId;
        }
      }
    }
  }

  const std::string CWidgetManagerImpl::m_sDbPath = staff::CRuntime::Inst().GetComponentHome("widget") + "/db/";
}
