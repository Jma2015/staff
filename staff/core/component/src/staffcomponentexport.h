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

#ifndef _STAFFCOMPONENTEXPORT_H_
#define _STAFFCOMPONENTEXPORT_H_

#ifdef WIN32
  #ifndef __MINGW32__
    #pragma warning(disable: 4786 4251)
  #endif

  #ifdef STAFF_COMPONENT_DLL_EXPORTS
    #define STAFF_COMPONENT_EXPORT __declspec(dllexport)
  #else
    #define STAFF_COMPONENT_EXPORT __declspec(dllimport)
  #endif

  #ifndef STAFF_DEPRECATED
    #ifndef __MINGW32__
      #define STAFF_DEPRECATED(Replacement) _CRT_INSECURE_DEPRECATE(Replacement)
    #else
      #define STAFF_DEPRECATED(Replacement)
    #endif
  #endif

#else
  #define STAFF_COMPONENT_EXPORT 
  
  #ifndef STAFF_DEPRECATED
    #define STAFF_DEPRECATED(Replacement)
  #endif
#endif

#endif // _STAFFCOMPONENTEXPORT_H_
