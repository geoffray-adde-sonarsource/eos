// ----------------------------------------------------------------------
// File: TransferQueue.hh
// Author: Andreas-Joachim Peters - CERN
// ----------------------------------------------------------------------

/************************************************************************
 * EOS - the CERN Disk Storage System                                   *
 * Copyright (C) 2011 CERN/Switzerland                                  *
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

/**
 * @file   TransferQueue.hh
 *
 * @brief  Base class for transfer queues.
 *
 *
 */

#ifndef __EOSCOMMON_TRANSFERQUEUE_HH__
#define __EOSCOMMON_TRANSFERQUEUE_HH__

/*----------------------------------------------------------------------------*/
#include "common/Namespace.hh"
#include "common/TransferJob.hh"
#include "common/FileSystem.hh"
#include "common/Locators.hh"
/*----------------------------------------------------------------------------*/
#include <string>
/*----------------------------------------------------------------------------*/

namespace eos
{
namespace mq
{
class MessagingRealm;
class SharedQueueWrapper;
}
}

namespace qclient
{
class SharedManager;
class SharedDeque;
}

EOSCOMMONNAMESPACE_BEGIN

class FileSystem;

//------------------------------------------------------------------------------
//! Class implementing the base class of a transfer queue used in FST & MGM
//------------------------------------------------------------------------------
class TransferQueue
{
private:
  //----------------------------------------------------------------------------
  //! Reference to the underlying shared queue maintained by the shared object manager
  //! Usage of this object requires a read lock on the shared object manager and the hash has to be validated!
  //----------------------------------------------------------------------------
  mq::MessagingRealm *mRealm;
  TransferQueueLocator mLocator;
  bool mBroadcast;

  //----------------------------------------------------------------------------
  //! Count number of jobs executed + mutex
  //----------------------------------------------------------------------------
  std::atomic<unsigned long long> mJobGetCount;

public:
  //----------------------------------------------------------------------------
  //! Constructor
  //----------------------------------------------------------------------------
  TransferQueue(const TransferQueueLocator& locator,
                mq::MessagingRealm* realm, bool bc2mgm);

  //----------------------------------------------------------------------------
  //! Destructor
  //----------------------------------------------------------------------------
  virtual ~TransferQueue();

  //----------------------------------------------------------------------------
  //! Get queue path
  //----------------------------------------------------------------------------
  std::string getQueuePath() const;

  //----------------------------------------------------------------------------
  //! Add a transfer job to the queue
  //----------------------------------------------------------------------------
  bool Add(eos::common::TransferJob* job);

  //----------------------------------------------------------------------------
  //! Get a transfer job from the queue
  //----------------------------------------------------------------------------
  std::unique_ptr<eos::common::TransferJob> Get();

  //----------------------------------------------------------------------------
  //! Get the count of retrieved transfers
  //----------------------------------------------------------------------------
  unsigned long long
  GetJobCount()
  {
    return mJobGetCount;
  }

  //----------------------------------------------------------------------------
  //! Increment the count of retrieved transfers
  //----------------------------------------------------------------------------

  void
  IncGetJobCount()
  {
    mJobGetCount++;
  }

  //----------------------------------------------------------------------------
  //! Get the current size of the queue
  //----------------------------------------------------------------------------
  size_t Size();

  // ---------------------------------------------------------------------------
  //! Clear all jobs from the queue
  // ---------------------------------------------------------------------------
  void Clear();

};

EOSCOMMONNAMESPACE_END

#endif
