// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#ifndef _SERVICEFACTORY_H_
#define _SERVICEFACTORY_H_

#include <typeinfo>
#include <string>

//! service factory
class CServiceFactory
{
public:
  //!         get instance of service factory
  static CServiceFactory& Inst();

  //!         allocate new object for work with service
  /*! example:
      CCalculator* pCalculator = CServiceFactory::Inst().GetService<CCalculator>("http://localhost:9090/axis2/services/Calculator")

      \param  sServiceUri - service URI
      \param  sSessionId - session identifier
      \return pointer to object for work with service
      */
  template<typename TServiceClientBaseType>
  TServiceClientBaseType* GetService( const std::string& sServiceUri = "",
                                      const std::string& sSessionId = "" )
  {
    return reinterpret_cast<TServiceClientBaseType*>(AllocateClient(typeid(TServiceClientBaseType).name(), sServiceUri, sSessionId));
  }

private:
  //! private constructor
  CServiceFactory();

  void* AllocateClient(const std::string& sClientType, const std::string& sServiceUri, const std::string& sSessionId);

private:
  static CServiceFactory* m_pInst;  //!< instance
};

#endif // _SERVICEFACTORY_H_
