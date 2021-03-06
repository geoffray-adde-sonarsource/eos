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

//------------------------------------------------------------------------------
//! @author Georgios Bitzes <georgios.bitzes@cern.ch>
//! @brief Class for scanning through all file metadata
//------------------------------------------------------------------------------

#pragma once
#include "namespace/Namespace.hh"
#include "proto/FileMd.pb.h"
#include <qclient/structures/QLocalityHash.hh>
#include <folly/futures/Future.h>

namespace qclient {
  class QClient;
}

EOSNSNAMESPACE_BEGIN

//------------------------------------------------------------------------------
//! FileScannerPrimitive class - no support for full paths
//------------------------------------------------------------------------------
class FileScannerPrimitive {
public:
  //----------------------------------------------------------------------------
  //! Constructor
  //----------------------------------------------------------------------------
  FileScannerPrimitive(qclient::QClient &qcl);

  //----------------------------------------------------------------------------
  //! Is the iterator valid?
  //----------------------------------------------------------------------------
  bool valid() const;

  //----------------------------------------------------------------------------
  //! Advance iterator - only call when valid() == true
  //----------------------------------------------------------------------------
  void next();

  //----------------------------------------------------------------------------
  //! Is there an error?
  //----------------------------------------------------------------------------
  bool hasError(std::string &err) const;

  //----------------------------------------------------------------------------
  //! Get current element
  //----------------------------------------------------------------------------
  bool getItem(eos::ns::FileMdProto &item);

  //----------------------------------------------------------------------------
  //! Get number of elements scanned so far
  //----------------------------------------------------------------------------
  uint64_t getScannedSoFar() const;

private:
  qclient::QLocalityHash::Iterator mIterator;
  std::string mError;
  uint64_t mScanned = 0;
};

//------------------------------------------------------------------------------
//! FileScanner class - optional support for full paths
//------------------------------------------------------------------------------
class FileScanner {
public:
  //----------------------------------------------------------------------------
  //! Constructor
  //----------------------------------------------------------------------------
  FileScanner(qclient::QClient &qcl, bool fullPaths = false);

  //----------------------------------------------------------------------------
  //! Is the iterator valid?
  //----------------------------------------------------------------------------
  bool valid() const;

  //----------------------------------------------------------------------------
  //! Advance iterator - only call when valid() == true
  //----------------------------------------------------------------------------
  void next();

  //----------------------------------------------------------------------------
  //! Is there an error?
  //----------------------------------------------------------------------------
  bool hasError(std::string &err) const;

  //----------------------------------------------------------------------------
  //! Return type of getItem
  //----------------------------------------------------------------------------
  struct Item {
    eos::ns::FileMdProto proto;
    folly::Future<std::string> fullPath;

    Item() : proto(), fullPath("") {}

    Item(eos::ns::FileMdProto &&pr, folly::Future<std::string> &&path)
    : proto(std::move(pr)), fullPath(std::move(path)) {}
  };

  //----------------------------------------------------------------------------
  //! Get current element
  //----------------------------------------------------------------------------
  bool getItem(eos::ns::FileMdProto &proto, Item *item = nullptr);

  //----------------------------------------------------------------------------
  //! Get number of elements scanned so far
  //----------------------------------------------------------------------------
  uint64_t getScannedSoFar() const;

private:
  //----------------------------------------------------------------------------
  //! Ensure our item deque contanis a sufficient number of pending items
  //----------------------------------------------------------------------------
  void ensureItemDequeFull();

  FileScannerPrimitive mScanner;
  qclient::QClient &mQcl;
  bool mFullPaths;
  bool mActive;

  std::deque<Item> mItemDeque;
  uint64_t mScanned = 0;
};





EOSNSNAMESPACE_END
