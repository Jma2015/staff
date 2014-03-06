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

#include <string>
#include <staff/utils/Log.h>
#include <staff/utils/SharedPtr.h>
#include <staff/common/Exception.h>
#include <staff/xml/Element.h>
#include <staff/component/ServiceInstanceManager.h>
#include <staff/component/ComponentConfig.h>
#include <staff/component/ComponentConfigManager.h>
#include "SubService.h"
#include "CalcServiceImpl.h"

namespace samples
{
  namespace calc
  {
    CalcServiceImpl::CalcServiceImpl():
      m_pConfig(NULL)
    {
    }

    CalcServiceImpl::~CalcServiceImpl()
    {
    }

    int CalcServiceImpl::Add(int nA, int nB) const
    {
      return nA + nB;  // result
    }

    int CalcServiceImpl::Sub(int nA, int nB) const
    {
      staff::SharedPtr<SubService> rpSubService =
        staff::ServiceInstanceManager::Inst().ServiceInstance(this, "samples.calc.SubService");

      STAFF_ASSERT(rpSubService, "Cannot get service [samples.calc.SubService]");

      return rpSubService->Sub(nA, nB);
    }

    void CalcServiceImpl::SetMem(int nMem)
    {
      m_pConfig->Config().CreateChildElementOnce("Mem").SetValue(nMem);
    }

    int CalcServiceImpl::GetMem() const
    {
      staff::xml::Element& rMemElement = m_pConfig->Config().CreateChildElementOnce("Mem");
      if (rMemElement.IsTextNull()) // if there is no saved mem
      {
        rMemElement.SetValue(0); // setting default value
      }
      return rMemElement.GetValue();
    }

    void CalcServiceImpl::OnCreate()
    {
      m_pConfig = &staff::ComponentConfigManager::Inst().GetComponentConfig(this);
    }

    void CalcServiceImpl::OnDestroy()
    {
      m_pConfig->SaveConfig();
    }
  }
}