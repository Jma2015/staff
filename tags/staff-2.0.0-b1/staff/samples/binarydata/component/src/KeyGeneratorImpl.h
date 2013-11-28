// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#ifndef _KeyGeneratorImpl_h_
#define _KeyGeneratorImpl_h_

#include "KeyGenerator.h"

namespace samples
{
namespace binarydata
{

  //! implementation of sample component service, to demonstare how to pass binary data over SOAP
  class KeyGeneratorImpl: public KeyGenerator
  {
  public:
    KeyGeneratorImpl();
    virtual ~KeyGeneratorImpl();
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual staff::hexBinary GenerateHexKey(unsigned uSourceKey);
    virtual staff::base64Binary GenerateBase64Key(unsigned uSourceKey);
    virtual bool ValidateHexKey(const staff::hexBinary& rKey);
    virtual bool ValidateBase64Key(const staff::base64Binary& rKey);

  protected:
    void GenerateKey(unsigned uSourceKey, staff::ByteArray& rResult);
    bool ValidateKey(const staff::ByteArray& rResult);
    short Checksum(const std::string& sBuffer);

  private:
    static const std::string m_sSignature;
  };
}
}


#endif // _KeyGeneratorImpl_h_
