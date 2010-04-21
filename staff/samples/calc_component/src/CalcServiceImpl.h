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

#ifndef _CalcServiceImpl_h_
#define _CalcServiceImpl_h_

#include "CalcService.h"

namespace staff
{
  class CComponentConfig;
}

namespace samples
{
  namespace calc
  {
    class CCalcServiceImpl: public CCalcService
    {
    public:
      CCalcServiceImpl();
      virtual ~CCalcServiceImpl();
      virtual int Add(int nA, int nB) const;
      virtual int Sub(int nA, int nB) const;
      virtual void SetMem(int nMem);
      virtual int GetMem() const;

    protected:
      virtual void OnCreate();
      virtual void OnDestroy();

    private:
      mutable staff::CComponentConfig* m_pConfig;
    };
  }
}

#endif // _CalcServiceImpl_h_
