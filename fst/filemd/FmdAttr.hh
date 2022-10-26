//------------------------------------------------------------------------------
//! @file FmdAttr.hh
//! @author Abhishek Lekshmanan - CERN
//------------------------------------------------------------------------------

/************************************************************************
 * EOS - the CERN Disk Storage System                                   *
 * Copyright (C) 2019 CERN/Switzerland                                  *
 *                                                                      *
 * This program is free software: you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 ************************************************************************/

#pragma once
#include "fst/filemd/FmdHandler.hh"

namespace eos::fst
{

class FSPathHandler;

class FileIo;

class FmdAttrHandler final: public FmdHandler
{
public:
  // We don't maintain any local other than the mutexes held by the parent class
  ~FmdAttrHandler() = default;

  FmdAttrHandler(std::unique_ptr<FSPathHandler>&& _FSPathHandler);

  fmd_handler_t get_type() override
  {
    return fmd_handler_t::ATTR;
  }

  void LocalDeleteFmd(eos::common::FileId::fileid_t fid,
                      eos::common::FileSystem::fsid_t fsid,
                      bool drop_file = false) override;

  bool Commit(eos::common::FmdHelper* fmd, bool lockit = false,
              std::string* path = nullptr) override;

  std::unique_ptr<eos::common::FmdHelper>
  LocalGetFmd(eos::common::FileId::fileid_t fid,
              eos::common::FileSystem::fsid_t fsid,
              bool force_retrieve = false, bool do_create = false,
              uid_t uid = 0, gid_t gid = 0,
              eos::common::LayoutId::layoutid_t layoutid = 0) override;

  bool GetInconsistencyStatistics(
    eos::common::FileSystem::fsid_t fsid,
    std::map<std::string, size_t>& statistics,
    std::map<std::string, std::set<eos::common::FileId::fileid_t>>& fidset)
  override;


  bool UpdateInconsistencyStat(const std::string& path,
                               std::map<std::string, size_t>& statistics,
                               std::map<std::string, std::set<eos::common::FileId::fileid_t>>& fidset);

  std::pair<bool, eos::common::FmdHelper>
  LocalRetrieveFmd(eos::common::FileId::fileid_t fid,
                   eos::common::FileSystem::fsid_t fsid,
                   std::string* path=nullptr) override;

  std::pair<bool, eos::common::FmdHelper>
  LocalRetrieveFmd(const std::string& path);


private:
  std::unique_ptr<FSPathHandler> mFSPathHandler;

  bool LocalPutFmd(eos::common::FileId::fileid_t fid,
                   eos::common::FileSystem::fsid_t fsid,
                   const eos::common::FmdHelper& fmd) override;

  int CreateFile(FileIo* fio);



  bool LocalPutFmd(const std::string& path,
                   const eos::common::FmdHelper& fmd);



  void LocalDeleteFmd(const std::string& path, bool drop_file);

  bool ResetDiskInformation(eos::common::FileSystem::fsid_t fsid) override;
  bool ResetMgmInformation(eos::common::FileSystem::fsid_t fsid) override;

  void SetSyncStatus(eos::common::FileSystem::fsid_t, bool) override {}


};

static constexpr auto gFmdAttrName = "user.eos.fmd";

} // namespace eos::fst