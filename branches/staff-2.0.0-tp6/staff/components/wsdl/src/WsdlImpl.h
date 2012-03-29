// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#ifndef _WsdlImpl_h_
#define _WsdlImpl_h_

#include "Wsdl.h"

namespace staff
{
namespace wsdl
{

  //! implementation of REST service to get the WSDL
  class WsdlImpl: public Wsdl
  {
  public:
    virtual void Get(const std::string& sComponent, const std::string& sFileName, Operation& rOperation);
  };
}
}


#endif // _WsdlImpl_h_