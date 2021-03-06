//------------------------------------------------------------------------------
// File: security-checker.cc
// Author: Georgios Bitzes - CERN
//------------------------------------------------------------------------------

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

#include "auth/SecurityChecker.hh"
#include <gtest/gtest.h>

TEST(SecurityChecker, BasicSanity)
{
  JailInformation localJail;
  localJail.sameJailAsThisPid = true;

  SecurityChecker checker(false);
  checker.inject(localJail.id, "/tmp/ayy/lmao", 1337, 0455, {42, 42} );
  ASSERT_EQ(checker.lookup(localJail, "/tmp/aaa", 1000, 1000),
            SecurityChecker::Info(CredentialState::kCannotStat, {0, 0} ));
  ASSERT_EQ(checker.lookup(localJail, "/tmp/ayy/lmao", 1000, 1000),
            SecurityChecker::Info(CredentialState::kBadPermissions, {0, 0} ));
  ASSERT_EQ(checker.lookup(localJail, "/tmp/ayy/lmao", 1337, 1000),
            SecurityChecker::Info(CredentialState::kBadPermissions, {0, 0} ));
  checker.inject(localJail.id, "/tmp/123", 1234, 0400, {42, 42} );
  ASSERT_EQ(checker.lookup(localJail, "/tmp/123", 1000, 1000),
            SecurityChecker::Info(CredentialState::kBadPermissions, {0, 0} ));
  ASSERT_EQ(checker.lookup(localJail, "/tmp/123", 1234, 1000),
            SecurityChecker::Info(CredentialState::kOk, {42, 42} ));
  ASSERT_EQ(checker.lookup(localJail, "/tmp/234", 1234, 1000),
            SecurityChecker::Info(CredentialState::kCannotStat, {0, 0} ));
  checker.inject(localJail.id, "/tmp/123", 1111, 0700, {37, 37} );
  ASSERT_EQ(checker.lookup(localJail, "/tmp/123", 1111, 1000),
            SecurityChecker::Info(CredentialState::kOk, {37, 37} ));
  ASSERT_EQ(checker.lookup(localJail, "/tmp/123", 1112, 1000),
            SecurityChecker::Info(CredentialState::kBadPermissions, {0, 0} ));
}
