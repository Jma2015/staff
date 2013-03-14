// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Client skeleton

#include <memory>
#include <staff/utils/Log.h>
#include <staff/common/logoperators.h>
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>
#include "test.h"


int main(int /*nArgs*/, const char* /*paszArgs*/[])
{
  try
  {
    std::auto_ptr< ::Test171 > pTest171(::staff::ServiceFactory::Inst().GetService< ::Test171 >());

    STAFF_ASSERT(pTest171.get(), "Cannot get client for service Test171!");

    // Invoke Your service here:

    // staff::Optional< ::testEnum > testEnum;
    // pTest171->test(testEnum);
    // staff::LogInfo() << "test called";
  }
  STAFF_CATCH_ALL

  return 0;
}
