// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#include <iostream>
#include "SoapArrayServiceImpl.h"

namespace samples
{
namespace soaparray
{

static unsigned nRecursion = 0; // not reentrant but ok for this sample

template <typename Type>
std::ostream& operator<<(std::ostream& rStream, const staff::Array<Type>& rArray)
{
  rStream << "\n";
  for (unsigned i = 0; i < nRecursion; ++i)
  {
    rStream << "  ";
  }
  rStream << "{\n";
  ++nRecursion;
  for (unsigned i = 0; i < nRecursion; ++i)
  {
    rStream << "  ";
  }

  const unsigned nSize = rArray.GetSize();
  rStream << "Array size: " << nSize << ", IsSparse: " << rArray.IsSparse() << " Items: ";
  for (unsigned i = 0; i < nSize; ++i)
  {
    if (rArray.IsEmpty(i))
    {
      rStream << "[(empty)]";
    }
    else
    {
      rStream << "[" << rArray[i] << "]";
    }
  }

  rStream << "\n";
  --nRecursion;
  for (unsigned i = 0; i < nRecursion; ++i)
  {
    rStream << "  ";
  }
  rStream << "}\n";
  return rStream;
}

SoapArrayServiceImpl::SoapArrayServiceImpl()
{
}

SoapArrayServiceImpl::~SoapArrayServiceImpl()
{
}

void SoapArrayServiceImpl::OnCreate()
{
  // this function is called when service instance is created and registered
}

void SoapArrayServiceImpl::OnDestroy()
{
  // this function is called immediately before service instance destruction
}

void SoapArrayServiceImpl::SetIntArray(const staff::Array<int>& rArray)
{
//  const unsigned nSize = rArray.GetSize();
//  std::cout << "Array size: " << nSize << ", IsSparse: " << rArray.IsSparse() << " Items: ";
//  for (unsigned i = 0; i < nSize; ++i)
//  {
//    if (rArray.IsEmpty(i))
//    {
//      std::cout << "[(empty)]";
//    }
//    else
//    {
//      std::cout << "[" << rArray[i] << "]";
//    }
//  }
//  std::cout << std::endl;
  std::cout << rArray;
}

staff::Array<int> SoapArrayServiceImpl::GetIntArray() const
{
  staff::Array<int> tResult(5);

  tResult[0] = 1;
  tResult[1] = 2;
  tResult[2] = 3;
  tResult[3] = 4;
  tResult[4] = 5;

  return tResult;  // result
}

staff::Array<int> SoapArrayServiceImpl::GetSparseIntArray() const
{
  staff::Array<int> tResult(5);

  tResult[0] = 1;
  tResult[2] = 3;
  tResult[4] = 5;

  return tResult;  // result
}

void SoapArrayServiceImpl::SetData(const Data& rData)
{
  // TODO: place your code here
}

Data SoapArrayServiceImpl::GetData() const
{
  Data tResult;
  // TODO: place your code here

  return tResult;  // result
}

Data SoapArrayServiceImpl::GetSparseData() const
{
  Data tResult;
  // TODO: place your code here

  return tResult;  // result
}

void SoapArrayServiceImpl::SetDataArray(const ::samples::soaparray::DataArray& rDataArray)
{
  // TODO: place your code here
}

::samples::soaparray::DataArray SoapArrayServiceImpl::GetDataArray() const
{
  ::samples::soaparray::DataArray tResult;
  // TODO: place your code here

  return tResult;  // result
}

void SoapArrayServiceImpl::SetArrayOfIntArray(const ::samples::soaparray::ArrayOfIntArray& rArrayOfIntArray)
{
  // TODO: place your code here
}

::samples::soaparray::ArrayOfIntArray SoapArrayServiceImpl::GetArrayOfIntArray() const
{
  ::samples::soaparray::ArrayOfIntArray tResult;
  // TODO: place your code here

  return tResult;  // result
}

::samples::soaparray::ArrayOfIntArray SoapArrayServiceImpl::GetSparseArrayOfIntArray() const
{
  ::samples::soaparray::ArrayOfIntArray tResult;
  // TODO: place your code here

  return tResult;  // result
}

void SoapArrayServiceImpl::SetArrayOfDataArray(const ::samples::soaparray::ArrayOfDataArray& rArrayOfDataArray)
{
  // TODO: place your code here
}

::samples::soaparray::ArrayOfDataArray SoapArrayServiceImpl::GetArrayOfDataArray() const
{
  ::samples::soaparray::ArrayOfDataArray tResult;
  // TODO: place your code here

  return tResult;  // result
}

void SoapArrayServiceImpl::SetArrayOfArrayOfIntArray(const ::samples::soaparray::ArrayOfArrayOfIntArray& rArrayOfArrayOfIntArray)
{
  std::cout << rArrayOfArrayOfIntArray;
}

::samples::soaparray::ArrayOfArrayOfIntArray SoapArrayServiceImpl::GetArrayOfArrayOfIntArray() const
{
  ::samples::soaparray::ArrayOfArrayOfIntArray tResult(2);

  tResult[0].Resize(2);

  tResult[0][0].Resize(3);
  tResult[0][0][0] = 111;
  tResult[0][0][1] = 112;
  tResult[0][0][2] = 113;

  tResult[0][1].Resize(2);
  tResult[0][1][0] = 121;
  tResult[0][1][1] = 122;


  tResult[1].Resize(2);

  tResult[1][0].Resize(2);
  tResult[1][0][0] = 211;
  tResult[1][0][1] = 212;

  tResult[1][1].Resize(3);
  tResult[1][1][0] = 221;
  tResult[1][1][1] = 222;
  tResult[1][1][2] = 223;

  return tResult;  // result
}

::samples::soaparray::ArrayOfArrayOfIntArray SoapArrayServiceImpl::GetSparseArrayOfArrayOfIntArray() const
{
  ::samples::soaparray::ArrayOfArrayOfIntArray tResult(3);

  tResult[0].Resize(2);

  tResult[0][0].Resize(3);
  tResult[0][0][2] = 113;

  tResult[0][1].Resize(2);
  tResult[0][1][0] = 121;


  tResult[2].Resize(2);

  tResult[2][0].Resize(2);
  tResult[2][0][0] = 211;
  tResult[2][0][1] = 212;

  tResult[2][1].Resize(3);
  tResult[2][1][1] = 222;
  tResult[2][1][2] = 223;


  return tResult;  // result
}

staff::Array<int> SoapArrayServiceImpl::Get2dIntArray() const
{
  staff::Array<int> tResult(2, 3);
  tResult.At(0, 0) = 11;
  tResult.At(0, 1) = 12;
  tResult.At(0, 2) = 13;
  tResult.At(1, 0) = 21;
  tResult.At(1, 1) = 22;
  tResult.At(1, 2) = 23;

  return tResult;
}

staff::Array<int> SoapArrayServiceImpl::Get2dSparseIntArray() const
{
  staff::Array<int> tResult(2, 4);
  tResult.At(0, 0) = 11;
  tResult.At(0, 2) = 13;
  tResult.At(1, 1) = 22;
  tResult.At(1, 2) = 23;

  return tResult;
}

staff::Array< staff::Array<int> > SoapArrayServiceImpl::GetComplex2dIntArray() const
{
  staff::Array< staff::Array<int> > tResult(2);

  tResult[0].Resize(3, 2);
  tResult[0].At(0, 0) = 111;
  tResult[0].At(0, 1) = 112;
  tResult[0].At(1, 0) = 121;
  tResult[0].At(1, 1) = 122;
  tResult[0].At(2, 0) = 131;
  tResult[0].At(2, 1) = 132;

  tResult[1].Resize(3, 2);
  tResult[1].At(0, 0) = 211;
  tResult[1].At(0, 1) = 212;
  tResult[1].At(1, 0) = 221;
  tResult[1].At(1, 1) = 222;
  tResult[1].At(2, 0) = 231;
  tResult[1].At(2, 1) = 232;

  return tResult;
}

staff::Array< staff::Array<int> > SoapArrayServiceImpl::GetComplex2dSparseIntArray() const
{
  staff::Array< staff::Array<int> > tResult(3);

  tResult[0].Resize(3, 2);
  tResult[0].At(0, 0) = 111;
  tResult[0].At(1, 0) = 121;
  tResult[0].At(1, 1) = 122;
  tResult[0].At(2, 1) = 132;

  tResult[2].Resize(3, 2);
  tResult[2].At(0, 0) = 311;
  tResult[2].At(0, 1) = 312;
  tResult[2].At(1, 0) = 321;
  tResult[2].At(1, 1) = 322;
  tResult[2].At(2, 0) = 331;
  tResult[2].At(2, 1) = 332;

  return tResult;
}


}
}

