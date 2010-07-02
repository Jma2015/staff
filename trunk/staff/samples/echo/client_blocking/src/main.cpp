// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Client skeleton

#include <memory>
#include <rise/common/Log.h>
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>
#include "Echo.h"

// callbacks


int main(int nArgs, const char* paszArgs[])
{
  try
  {
    std::auto_ptr< ::samples::Echo > pEcho(::staff::CServiceFactory::Inst().GetService< ::samples::Echo >());

    RISE_ASSERTES(pEcho.get(), rise::CLogicNoItemException, "Cannot get client for service samples.Echo!");

    // Invoke Your service here:

    std::string tEchoStringResult = pEcho->EchoString("test");
    rise::LogInfo() << "EchoString result: " << tEchoStringResult;
  }
  catch(const staff::CRemoteException& rEx)
  {
    rise::LogError() << rEx.GetDescr();
  }
  RISE_CATCH_ALL

  return 0;
}

