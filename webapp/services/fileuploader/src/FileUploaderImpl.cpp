// This file generated by staff_codegen
// Service Implementation

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <rise/common/ExceptionTemplate.h>
#include <rise/common/exmacros.h>
#include <rise/tools/FileFind.h>
#include <rise/string/String.h>
#include <rise/xml/XMLNode.h>
#include <rise/xml/XMLDocument.h>
#include <staff/common/Runtime.h>
#include "FileUploaderImpl.h"

namespace webapp
{

  CFileUploaderImpl::CFileUploaderImpl()
  {
  }

  CFileUploaderImpl::~CFileUploaderImpl()
  {
  }

  void CFileUploaderImpl::Move(const std::string& sFileName, const std::string& sPathTo)
  {
    RISE_ASSERTES(sFileName.size() != 0, rise::CLogicNoItemException, "Filename is not set");
    RISE_ASSERTES(sPathTo.size() != 0, rise::CLogicNoItemException, "Target path is not set");

    const std::string& sFilePathFrom = m_sUploadDir + sFileName;
    const std::string& sFilePathTo = sPathTo + "/" + sFileName;
    int nResult = 0;

    mkdir(sPathTo.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

    unlink(sFilePathTo.c_str());
    
    nResult = rename(sFilePathFrom.c_str(), sFilePathTo.c_str());
    if (nResult == -1)
    {
      if (errno == EXDEV)
      {  // different devices: need to copy file, and then delete;
        char szBuffer[40960];
        std::ifstream fsIn;
        std::ofstream fsOut;

        fsIn.open(sFilePathFrom.c_str(), std::ios::in | std::ios::binary);
        if (!fsIn.good())
        {
          RISE_THROWS(rise::CFileOpenException, "Can\'t open source file path: " + sFilePathFrom);
        }

        fsOut.open(sFilePathTo.c_str(), std::ios::out | std::ios::binary);
        if (!fsOut.good())
        {
          fsIn.close();
          RISE_THROWS(rise::CFileOpenException, "Can\'t open destination file path: " + sFilePathTo);
        }

        while (!fsIn.eof())
        {
          fsIn.read(szBuffer, sizeof(szBuffer));
          fsOut.write(szBuffer, fsIn.gcount());
        }

        fsIn.close();
        fsOut.close();

        unlink(sFilePathFrom.c_str());
      }
      else
      {
        RISE_THROWS(rise::CLogicNoItemException, strerror(errno));
      }
    }
    m_sLastSaveToDir = sPathTo;
  }

  void CFileUploaderImpl::Unpack(const std::string& sFileName, const std::string& sPathTo)
  {
    RISE_ASSERTES(sFileName.size() != 0, rise::CLogicNoItemException, "Filename is not set");
    RISE_ASSERTES(sPathTo.size() != 0, rise::CLogicNoItemException, "Target path is not set");

    const std::string& sFilePathFrom = m_sUploadDir + sFileName;
    std::string::size_type nSize = sFilePathFrom.size();
    std::string sUnpackCmd;

    RISE_ASSERTES(!m_tUnpacker.GetStatus() != CFileUnpacker::ERunning, rise::CLogicAlreadyExistsException, "Unpacking already running");

    if (m_mUnpackers.size() == 0)
    { // load config
      rise::xml::CXMLDocument tDocConf;
      const rise::xml::CXMLNode& rNodeRoot = tDocConf.GetRoot();
      
      tDocConf.LoadFromFile(staff::CRuntime::Inst().GetComponentHome("fileuploader") + "/config/FileUploader.xml");

      const rise::xml::CXMLNode& rNodeUnpackers = rNodeRoot.Subnode("Unpackers");
      for (rise::xml::CXMLNode::TXMLNodeConstIterator itNode = rNodeUnpackers.NodeBegin();
            itNode != rNodeUnpackers.NodeEnd(); ++itNode)
      {
        const rise::xml::CXMLNode& rNodeUnpacker = *itNode;
        if (rNodeUnpacker.NodeType() == rise::xml::CXMLNode::ENTGENERIC &&
            rNodeUnpacker.NodeName() == "Unpacker")
        {
          m_mUnpackers[rNodeUnpacker["FileExt"].AsString()] = rNodeUnpacker["Cmd"].AsString();
        }
      }
    }

    for (TStringMap::const_iterator itUnpacker = m_mUnpackers.begin(); 
          itUnpacker != m_mUnpackers.end(); ++itUnpacker)
    {
      if (sFilePathFrom.substr(nSize - itUnpacker->first.size()) == itUnpacker->first)
      {
        sUnpackCmd = itUnpacker->second;
      }
    }

    RISE_ASSERTES(sUnpackCmd.size() != 0, rise::CLogicNoItemException, "Cannot detect unpacker for file \"" + sFileName + "\"");

    rise::StrReplace(sUnpackCmd, " ", "\t", true); // split parameters
    rise::StrReplace(sUnpackCmd, "$(SRC_FILE)", sFilePathFrom);
    rise::StrReplace(sUnpackCmd, "$(DST_DIR)", sPathTo);
    
    m_tUnpacker.Start(sUnpackCmd);
    m_sLastSaveToDir = sPathTo;
  }

  int CFileUploaderImpl::GetUnpackingStatus()
  {
    return m_tUnpacker.GetStatus();
  }

  TStringList CFileUploaderImpl::GetUnpackedFiles(const std::string& sMask)
  {
    TStringList lsFiles;
    RISE_ASSERTES(m_sLastSaveToDir.size() != 0, rise::CLogicNoItemException, "Last save dir is not set");

    GetUnpackedFiles(sMask, lsFiles);
    return lsFiles;
  }

  void CFileUploaderImpl::GetUnpackedFiles(const std::string& sMask, TStringList& rFiles, const std::string& sRelativeDir /*= ""*/)
  {
    TStringList lsDirs;
    TStringList lsFiles;
    rise::CFileFind::Find(m_sLastSaveToDir + "/" + sRelativeDir, lsDirs, "*", rise::CFileFind::EFA_DIR);
    for(TStringList::const_iterator itDir = lsDirs.begin(); itDir != lsDirs.end(); ++itDir)
    {
      GetUnpackedFiles(sMask, rFiles, (sRelativeDir.size() == 0) ? *itDir : (sRelativeDir + "/" + *itDir));
    }

    rise::CFileFind::Find(m_sLastSaveToDir + "/" + sRelativeDir, lsFiles, sMask, rise::CFileFind::EFA_FILE);
    for(TStringList::iterator itFile = lsFiles.begin(); itFile != lsFiles.end(); ++itFile)
    {
      rFiles.push_back((sRelativeDir.size() == 0) ? *itFile : (sRelativeDir + "/" + *itFile));
    }
  }

  void CFileUploaderImpl::Delete(const std::string& sFileName)
  {
    unlink((m_sUploadDir + sFileName).c_str());
  }

  const std::string CFileUploaderImpl::m_sUploadDir = "/tmp/upload/";
  CFileUploaderImpl::TStringMap CFileUploaderImpl::m_mUnpackers;
}

