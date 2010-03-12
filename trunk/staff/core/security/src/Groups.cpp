#include <rise/common/ExceptionTemplate.h>
#include <rise/common/exmacros.h>
#include <rise/common/Log.h>
#include "sqlite3.h"
#include "DbConn.h"
#include "Groups.h"

namespace staff
{
  namespace security
  {
    CGroups& CGroups::Inst()
    {
      if (!m_pInst)
      {
        m_pInst = new CGroups;
      }

      return *m_pInst;
    }

    void CGroups::FreeInst()
    {
      if (m_pInst)
      {
        delete m_pInst;
        m_pInst = NULL;
      }
    }

    void CGroups::GetById(int nId, SGroup& rstGroup)
    {
      sqlite3* pDb = CDbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description FROM groups WHERE id=?", -1, &pVm, NULL);
      RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        RISE_ASSERTS(sqlite3_step(pVm) == SQLITE_ROW, "Group with id is not found: " + std::string(sqlite3_errmsg(pDb)));

        rstGroup.nId = sqlite3_column_int(pVm, 0);
        const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
        RISE_ASSERTS(szTmp, "Failed to get group name");
        rstGroup.sName = szTmp;

        szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
        rstGroup.sDescription = szTmp != NULL ? szTmp : "";
      }
      catch(...)
      {
        RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
        throw;
      }

      RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void CGroups::GetByName(const std::string& sGroupName, SGroup& rstGroup)
    {
      sqlite3* pDb = CDbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description FROM groups WHERE name=?", -1, &pVm, NULL);
      RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_text(pVm, 1, sGroupName.c_str(), sGroupName.size(), SQLITE_STATIC);
        RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        RISE_ASSERTS(sqlite3_step(pVm) == SQLITE_ROW, "Group with name is not found: " + std::string(sqlite3_errmsg(pDb)));

        rstGroup.nId = sqlite3_column_int(pVm, 0);
        const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
        RISE_ASSERTS(szTmp, "Failed to get group name");
        rstGroup.sName = szTmp;

        szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
        rstGroup.sDescription = szTmp != NULL ? szTmp : "";
      }
      catch(...)
      {
        RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
        throw;
      }

      RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void CGroups::GetList(TGroupsList& rlsGroups)
    {
      sqlite3* pDb = CDbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description FROM groups", -1, &pVm, NULL);
      RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      rlsGroups.clear();

      try
      {
        // get data
        while (sqlite3_step(pVm) == SQLITE_ROW)
        {
          SGroup stGroup;
          stGroup.nId = sqlite3_column_int(pVm, 0);
          const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
          RISE_ASSERTS(szTmp, "Failed to get group name");
          stGroup.sName = szTmp;

          szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
          stGroup.sDescription = szTmp != NULL ? szTmp : "";
          rlsGroups.push_back(stGroup);
        }
      }
      catch(...)
      {
        RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
        throw;
      }

      RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void CGroups::Add(const std::string& sName, const std::string& sDescription, int& nId)
    {
      sqlite3* pDb = CDbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      // add group
      int nResult = sqlite3_prepare_v2(pDb, "INSERT INTO groups(name, description) VALUES(?, ?)", -1, &pVm, NULL);
      RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_text(pVm, 1, sName.c_str(), sName.size(), SQLITE_STATIC);
        RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        nResult = sqlite3_bind_text(pVm, 2, sDescription.c_str(), sDescription.size(), SQLITE_STATIC);
        RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        RISE_ASSERTS(sqlite3_step(pVm) == SQLITE_DONE, "Failed to create group: " + std::string(sqlite3_errmsg(pDb)));

        // get inserted group id
        nId = sqlite3_last_insert_rowid(pDb);
      }
      catch(...)
      {
        RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
        throw;
      }

      RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void CGroups::Remove(int nId)
    {
      sqlite3* pDb = CDbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      // remove group
      int nResult = sqlite3_prepare_v2(pDb, "DELETE FROM groups WHERE id=?", -1, &pVm, NULL);
      RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        RISE_ASSERTS(sqlite3_step(pVm) == SQLITE_DONE, "Failed to remove group: " + std::string(sqlite3_errmsg(pDb)));
      }
      catch(...)
      {
        RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
        throw;
      }

      RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void CGroups::SetDescription(int nId, const std::string& sDescription)
    {
      sqlite3* pDb = CDbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      // set group password
      int nResult = sqlite3_prepare_v2(pDb, "UPDATE groups SET description=? WHERE id=?", -1, &pVm, NULL);
      RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_text(pVm, 1, sDescription.c_str(), sDescription.size(), SQLITE_STATIC);
        RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        nResult = sqlite3_bind_int(pVm, 2, nId);
        RISE_ASSERTS(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        RISE_ASSERTS(sqlite3_step(pVm) == SQLITE_DONE, "Failed to set group description: " + std::string(sqlite3_errmsg(pDb)));
      }
      catch(...)
      {
        RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
        throw;
      }

      RISE_ASSERTS(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    CGroups::CGroups()
    {
    }

    CGroups::~CGroups()
    {
    }

    CGroups* CGroups::m_pInst = NULL;
  }
}
