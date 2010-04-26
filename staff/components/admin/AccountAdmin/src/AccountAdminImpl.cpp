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

#include <rise/common/Log.h>
#include <rise/common/exmacros.h>
#include <staff/common/Exception.h>
#include <staff/security/Users.h>
#include <staff/security/Groups.h>
#include <staff/security/UsersToGroups.h>
#include "AccountAdminImpl.h"

namespace staff
{
namespace admin
{

CAccountAdminImpl::CAccountAdminImpl()
{
}

CAccountAdminImpl::~CAccountAdminImpl()
{
}

::staff::admin::TUserList CAccountAdminImpl::GetUsers()
{
  ::staff::admin::TUserList tResult;
  staff::security::TUsersList lsSecUsers;

  staff::security::CUsers::Inst().GetList(lsSecUsers);

  for (staff::security::TUsersList::const_iterator itUser = lsSecUsers.begin();
      itUser != lsSecUsers.end(); ++itUser)
  {
    ::staff::admin::SUser tUser;
    tUser.nId = itUser->nId;
    tUser.sName = itUser->sName;
    tUser.sDescription = itUser->sDescription;

    tResult.push_back(tUser);
  }

  return tResult;  // result
}

::staff::admin::TGroupList CAccountAdminImpl::GetGroups()
{
  ::staff::admin::TGroupList tResult;
  staff::security::TGroupsList lsSecGroups;

  staff::security::CGroups::Inst().GetList(lsSecGroups);

  for (staff::security::TGroupsList::const_iterator itGroup = lsSecGroups.begin();
      itGroup != lsSecGroups.end(); ++itGroup)
  {
    ::staff::admin::SGroup tGroup;
    tGroup.nId = itGroup->nId;
    tGroup.sName = itGroup->sName;
    tGroup.sDescription = itGroup->sDescription;

    tResult.push_back(tGroup);
  }

  return tResult;  // result
}

::staff::admin::TIdList CAccountAdminImpl::GetUserGroups(int nUserId)
{
  ::staff::admin::TIdList tResult;
//  staff::security::TIntList lsGroups;
  staff::security::CUsersToGroups::Inst().GetUserGroups(nUserId, tResult);

  return tResult;  // result
}

int CAccountAdminImpl::AddUser(const std::string& sUserName, const std::string& sDescription)
{
  int tResult = -1;

  staff::security::CUsers::Inst().Add(sUserName, "", sDescription, tResult);

  return tResult;  // result
}

void CAccountAdminImpl::RemoveUser(int nUserId)
{
  staff::security::CUsers::Inst().Remove(nUserId);
}

void CAccountAdminImpl::SetUserPassword(int nUserId, const std::string& sPass)
{
  staff::security::CUsers::Inst().SetPassword(nUserId, sPass);
}

int CAccountAdminImpl::AddGroup(const std::string& sGroupName, const std::string& sDescription)
{
  int tResult = -1;

  staff::security::CGroups::Inst().Add(sGroupName, sDescription, tResult);

  return tResult;  // result
}

void CAccountAdminImpl::RemoveGroup(int nGroupId)
{
  staff::security::CGroups::Inst().Remove(nGroupId);
}

void CAccountAdminImpl::AddUserToGroup(int nUserId, int nGroupId)
{
  staff::security::CUsersToGroups::Inst().AddUserToGroup(nUserId, nGroupId);
}

void CAccountAdminImpl::AddUserToGroups(int nUserId, const ::staff::admin::TIdList& rlsGroupIds)
{
  for(::staff::admin::TIdList::const_iterator itGroupId = rlsGroupIds.begin();
      itGroupId != rlsGroupIds.end(); ++itGroupId)
  {
    staff::security::CUsersToGroups::Inst().AddUserToGroup(nUserId, *itGroupId);
  }
}

void CAccountAdminImpl::RemoveUserFromGroup(int nUserId, int nGroupId)
{
  staff::security::CUsersToGroups::Inst().RemoveUserFromGroup(nUserId, nGroupId);
}

void CAccountAdminImpl::RemoveUserFromGroups(int nUserId, const ::staff::admin::TIdList& rlsGroupIds)
{
  for(::staff::admin::TIdList::const_iterator itGroupId = rlsGroupIds.begin();
      itGroupId != rlsGroupIds.end(); ++itGroupId)
  {
    staff::security::CUsersToGroups::Inst().RemoveUserFromGroup(nUserId, *itGroupId);
  }
}

}
}