// ----------------------------------------------------------------------
// File: DbMap.cc
// Author: Geoffray Adde - CERN
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

#include "common/DbMap.hh"
#include "common/Namespace.hh"

EOSCOMMONNAMESPACE_BEGIN

thread_local bool DbMapT::tlIterating {false};
thread_local std::unique_ptr<DbMapT::TlogentryVec>
DbMapT::tlDbItList {new DbMapT::TlogentryVec()};
thread_local DbMapT::TlogentryVec::const_iterator DbMapT::tlDbIt;
/*-------------- IMPLEMENTATIONS OF STATIC MEMBER VARIABLES ------------------*/
set<string> DbMapT::gNames;
eos::common::RWMutex DbMapT::gNamesMutex;
eos::common::RWMutex DbMapT::gTimeMutex;
bool DbMapT::gInitialized = false;
size_t DbMapT::pDbIterationChunkSize = 10000;
/*----------------------------------------------------------------------------*/
typedef DbMapT DbMapLeveldb;

DbLogT::DbLogT()
{
  pDb = new TDbLogInterface();
  pMutex.SetBlocking(true);
}

DbLogT::DbLogT(const std::string dbfile, int volumeduration, int createperm,
               Toption* option)
{
  pDb = new TDbLogInterface(dbfile, volumeduration, createperm, option);
}

DbLogT::~DbLogT()
{
  RWMutexWriteLock lock(pMutex);
  delete pDb;
}

bool DbLogT::setDbFile(const std::string& dbname, int volumeduration,
                       int createperm, Toption* option)
{
  RWMutexWriteLock lock(pMutex);
  return pDb->setDbFile(dbname, volumeduration, createperm, (void*)option);
}

std::string DbLogT::getDbFile() const
{
  RWMutexWriteLock lock(pMutex);
  return pDb->getDbFile();
}

int DbLogT::getAll(TlogentryVec* retvec, size_t nmax,
                   Tlogentry* startafter) const
{
  int startsize = retvec->size();
  RWMutexReadLock lock(pMutex);
  pDb->getAll(retvec, nmax, startafter);
  return retvec->size() - startsize;
}

int DbLogT::getTail(int nentries, TlogentryVec* retvec) const
{
  int startsize = retvec->size();
  RWMutexReadLock lock(pMutex);
  pDb->getTail(nentries, retvec);
  return retvec->size() - startsize;
}

bool DbLogT::clear()
{
  RWMutexReadLock lock(pMutex);
  return pDb->clear();
}

std::string DbLogT::getDbType()
{
  return TDbLogInterface::getDbType();
}

EOSCOMMONNAMESPACE_END
