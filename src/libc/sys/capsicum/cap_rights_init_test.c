// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <testing.h>

TEST(cap_rights_init, example) {
  cap_rights_t rights;
  ASSERT_EQ(&rights, cap_rights_init(&rights));
  ASSERT_EQ(0, rights.__value);
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_READ));
  ASSERT_EQ(CAP_READ, rights.__value);
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_READ, CAP_WRITE));
  ASSERT_EQ(CAP_READ | CAP_WRITE, rights.__value);
}
