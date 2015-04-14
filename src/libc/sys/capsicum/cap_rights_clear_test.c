// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <testing.h>

TEST(cap_rights_clear, example) {
  // Initial rights sets.
  cap_rights_t rights1;
  ASSERT_EQ(&rights1,
            cap_rights_init(&rights1, CAP_READ, CAP_WRITE, CAP_CONNECT));
  cap_rights_t rights2;
  ASSERT_EQ(&rights2, cap_rights_init(&rights2, CAP_READ));
  ASSERT_FALSE(cap_rights_equals(&rights1, &rights2));

  // Sets should be identical after removing the mismatches.
  ASSERT_EQ(&rights1,
            cap_rights_clear(&rights1, CAP_WRITE, CAP_CONNECT, CAP_UNLINKAT));
  ASSERT_TRUE(cap_rights_equals(&rights1, &rights2));
}
