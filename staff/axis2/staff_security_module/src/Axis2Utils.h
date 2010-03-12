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

#ifndef _AXIS2UTILS_H_
#define _AXIS2UTILS_H_

#include <axis2_module_desc.h>
#include <axis2_msg_ctx.h>

#define AXIS2_UTILS_CHECK(expression) \
  if(!(expression)) { printf("error: %s[%d]: %s\n", GetBaseFile(__FILE__), __LINE__, #expression); return AXIS2_FAILURE; }

axis2_status_t GetServiceOperationPath(axis2_msg_ctx_t* pMsgCtx, const axutil_env_t* pEnv, axis2_char_t** psServiceOperationPath);
axis2_status_t GetSessionId(axis2_msg_ctx_t* pMsgCtx, const axutil_env_t* pEnv, const axis2_char_t** pszSessionId);
const axis2_char_t* GetParamValue(const axis2_char_t* szParam, axis2_module_desc_t* pModuleDesc, const axutil_env_t* pEnv);
const char* GetBaseFile(const char* szFilePath);

#endif // _AXIS2UTILS_H_


