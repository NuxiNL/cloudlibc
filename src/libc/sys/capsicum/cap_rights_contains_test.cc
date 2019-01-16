// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include "gtest/gtest.h"

TEST(cap_rights_contains, example) {
  {
    // rights1 is a superset of rights2.
    cap_rights_t rights1;
    ASSERT_EQ(&rights1,
              cap_rights_init(&rights1, CAP_READ, CAP_WRITE, CAP_FCNTL));
    cap_rights_t rights2;
    ASSERT_EQ(&rights2, cap_rights_init(&rights2, CAP_READ, CAP_WRITE));
    ASSERT_TRUE(cap_rights_contains(&rights1, &rights2));
  }

  {
    // rights1 is a subset of rights2.
    cap_rights_t rights1;
    ASSERT_EQ(&rights1, cap_rights_init(&rights1, CAP_READ, CAP_WRITE));
    cap_rights_t rights2;
    ASSERT_EQ(&rights2,
              cap_rights_init(&rights2, CAP_READ, CAP_WRITE, CAP_FCNTL));
    ASSERT_FALSE(cap_rights_contains(&rights1, &rights2));
  }

  {
    // rights1 and rights2 are identical.
    cap_rights_t rights1;
    ASSERT_EQ(&rights1, cap_rights_init(&rights1, CAP_READ, CAP_WRITE));
    cap_rights_t rights2;
    ASSERT_EQ(&rights2, cap_rights_init(&rights2, CAP_READ, CAP_WRITE));
    ASSERT_TRUE(cap_rights_contains(&rights1, &rights2));
  }
}
