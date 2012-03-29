// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#include <staff/common/Exception.h>
#include <staff/sqlite3/sqlite3.h>
#include "DbConn.h"
#include "ServiceFactory.h"
#include "Users.h"
#include "AccountsImpl.h"

namespace staff
{
namespace samples
{
namespace das
{
namespace services
{

AccountsImpl::AccountsImpl()
{
}

AccountsImpl::~AccountsImpl()
{
}

void AccountsImpl::OnCreate()
{
  DbConn::Open();
}

void AccountsImpl::OnDestroy()
{
  DbConn::Close();
}

::staff::samples::das::services::RolesList AccountsImpl::GetGroups()
{
  ::staff::samples::das::services::RolesList tResult;

  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description FROM groups", -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    // get data
    while (sqlite3_step(pVm) == SQLITE_ROW)
    {
      Role stUser;
      stUser.nId = sqlite3_column_int(pVm, 0);
      const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
      STAFF_ASSERT(szTmp, "Failed to get user name");
      stUser.sName = szTmp;

      szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
      stUser.sDescr = szTmp != NULL ? szTmp : "";
      tResult.push_back(stUser);
    }
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
  return tResult;  // result
}

::staff::samples::das::services::RolesList AccountsImpl::GetUsers()
{
  ::staff::samples::das::services::RolesList tResult;

  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description FROM users", -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    // get data
    while (sqlite3_step(pVm) == SQLITE_ROW)
    {
      Role stUser;
      stUser.nId = sqlite3_column_int(pVm, 0);
      const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
      STAFF_ASSERT(szTmp, "Failed to get user name");
      stUser.sName = szTmp;

      szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
      stUser.sDescr = szTmp != NULL ? szTmp : "";
      tResult.push_back(stUser);
    }
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
  return tResult;  // result
}

Role AccountsImpl::GetGroupById(int nId)
{
  Role stUser;
  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description FROM groups "
                                   "WHERE id=?", -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    nResult = sqlite3_bind_int(pVm, 1, nId);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    // get data
    STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_ROW, "User with id is not found");

    stUser.nId = sqlite3_column_int(pVm, 0);

    const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
    STAFF_ASSERT(szTmp, "Failed to get user name");
    stUser.sName = szTmp;

    szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
    stUser.sDescr = szTmp != NULL ? szTmp : "";
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
  return stUser;
}

int AccountsImpl::AddUser(const std::string& sName, const std::string& sPassword, const std::string& sDescr)
{
  int tResult;

  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  // add user
  int nResult = sqlite3_prepare_v2(pDb, "INSERT INTO users(name, password, description) VALUES(?, ?, ?)",
                                   -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    nResult = sqlite3_bind_text(pVm, 1, sName.c_str(), sName.size(), SQLITE_STATIC);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    nResult = sqlite3_bind_text(pVm, 2, sPassword.c_str(), sPassword.size(), SQLITE_STATIC);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    nResult = sqlite3_bind_text(pVm, 3, sDescr.c_str(), sDescr.size(), SQLITE_STATIC);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_DONE, "Failed to create user: " + std::string(sqlite3_errmsg(pDb)));

    // get inserted user id
    tResult = static_cast<int>(sqlite3_last_insert_rowid(pDb));
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));

  return tResult;  // result
}

IntList AccountsImpl::AddUsers(const UserToAddList& lsUsers)
{
  IntList tResult;

  for (UserToAddList::const_iterator it = lsUsers.begin(); it != lsUsers.end(); ++it)
  {
    tResult.push_back(AddUser(it->sName, it->sPassword, it->sDescr));
  }

  return tResult;
}

void AccountsImpl::RemoveUser(int nId)
{
  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  // remove user
  int nResult = sqlite3_prepare_v2(pDb, "DELETE FROM users WHERE id=?", -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    nResult = sqlite3_bind_int(pVm, 1, nId);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_DONE, "Failed to remove user: " + std::string(sqlite3_errmsg(pDb)));
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
}

void AccountsImpl::RemoveUserByName(const std::string& sName)
{
  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  // remove user
  int nResult = sqlite3_prepare_v2(pDb, "DELETE FROM users WHERE name=?", -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    nResult = sqlite3_bind_text(pVm, 1, sName.c_str(), sName.size(), SQLITE_STATIC);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_DONE, "Failed to remove user: " + std::string(sqlite3_errmsg(pDb)));
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
}

int AccountsImpl::GetUserIdByName(const std::string& sName)
{
  int nId = 0;
  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  int nResult = sqlite3_prepare_v2(pDb, "SELECT id FROM users WHERE name=?", -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    nResult = sqlite3_bind_text(pVm, 1, sName.c_str(), sName.size(), SQLITE_STATIC);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    // get data
    STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_ROW, "User with name is not found");

    nId = sqlite3_column_int(pVm, 0);
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
  return nId;
}

Role AccountsImpl::GetUser(int nId)
{
  Role stUser;
  sqlite3* pDb = DbConn::GetDb();
  sqlite3_stmt* pVm = NULL;

  int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description FROM users "
                                   "WHERE id=?", -1, &pVm, NULL);
  STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

  try
  {
    nResult = sqlite3_bind_int(pVm, 1, nId);
    STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

    // get data
    STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_ROW, "User with id is not found");

    stUser.nId = sqlite3_column_int(pVm, 0);

    const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
    STAFF_ASSERT(szTmp, "Failed to get user name");
    stUser.sName = szTmp;

    szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
    stUser.sDescr = szTmp != NULL ? szTmp : "";
  }
  catch(...)
  {
    sqlite3_finalize(pVm);
    throw;
  }

  STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
  return stUser;
}

Role AccountsImpl::GetUserSvc(int nId)
{
  Role stResult;

  // get datasource's proxy
  if (!m_tpUsersDatasource)
  {
    m_tpUsersDatasource = staff::das::samples::ServiceFactory::Inst().GetService("staff.das.samples.Users", this);
  }

  STAFF_ASSERT(m_tpUsersDatasource, "failed to get datasource");

  staff::das::samples::User stUser = m_tpUsersDatasource->GetUser(nId);

  stResult.nId = stUser.nId;
  stResult.sName = stUser.sName;
  stResult.sDescr = stUser.sDescr;

  return stResult;
}


}
}
}
}

