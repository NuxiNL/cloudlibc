// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <testing.h>

TEST(cap_rights_clear, example) {
  // Initial rights sets.
  cap_rights_t rights1;
  ASSERT_EQ(&rights1,
            cap_rights_init(&rights1, CAP_READ, CAP_WRITE, CAP_FCNTL));
  cap_rights_t rights2;
  ASSERT_EQ(&rights2, cap_rights_init(&rights2, CAP_READ));

  // Sets should be identical after removing the mismatches.
  ASSERT_EQ(&rights1,
            cap_rights_clear(&rights1, CAP_WRITE, CAP_FCNTL, CAP_UNLINKAT));
  ASSERT_EQ(rights1.__value, rights2.__value);
}
