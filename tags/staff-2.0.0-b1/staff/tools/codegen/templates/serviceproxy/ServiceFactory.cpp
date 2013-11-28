// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#include <staff/common/Exception.h>
#include <staff/utils/SharedPtr.h>
#include <staff/utils/PluginExport.h>
#include <staff/common/IService.h>
#foreach $(Project.Interfaces)
#include "$(Interface.FilePath)$(Interface.Name)Proxy.h"
#end
#include "ServiceFactory.h"

$(Project.OpeningNs)
  ServiceFactory::ServiceFactory()
  {
  }

  ServiceFactory::~ServiceFactory()
  {
  }

  ServiceFactory& ServiceFactory::Inst()
  {
    static ServiceFactory tInst;
    return tInst;
  }

  staff::IService* ServiceFactory::GetService(const std::string& sServiceNsName,
                                              const std::string& sSessionId,
                                              const std::string& sInstanceId)
  {
#foreach $(Project.Interfaces)
#foreach $(Interface.Classes)
    if (sServiceNsName == "$(Class.NsName.!dot)")
    {
      return new $(Class.NsName)Proxy(sSessionId, sInstanceId);
    }
    else
#end
#end
    {
      STAFF_THROW_ASSERT("Service [" + sServiceNsName + "] is not registered");
    }
  }

  staff::IService* ServiceFactory::GetService(const std::string& sServiceNsName,
                                              const staff::IService *pCurrentService)
  {
#foreach $(Project.Interfaces)
#foreach $(Interface.Classes)
    if (sServiceNsName == "$(Class.NsName.!dot)")
    {
      return new $(Class.NsName)Proxy(pCurrentService->GetSessionId(), pCurrentService->GetInstanceId());
    }
    else
#end
#end
    {
      STAFF_THROW_ASSERT("Service [" + sServiceNsName + "] is not registered");
    }
  }

$(Project.EndingNs)

