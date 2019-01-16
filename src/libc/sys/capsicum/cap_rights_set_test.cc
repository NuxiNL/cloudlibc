// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include "gtest/gtest.h"

TEST(cap_rights_set, example) {
  // Initial rights sets.
  cap_rights_t rights1;
  ASSERT_EQ(&rights1, cap_rights_init(&rights1));
  cap_rights_t rights2;
  ASSERT_EQ(&rights2, cap_rights_init(&rights2, CAP_READ, CAP_WRITE));

  // CAP_WRITE missing in rights1.
  ASSERT_EQ(&rights1, cap_rights_set(&rights1, CAP_READ));
  ASSERT_NE(rights1.__value, rights2.__value);

  // CAP_FCNTL missing in rights2.
  ASSERT_EQ(&rights1, cap_rights_set(&rights1, CAP_WRITE, CAP_FCNTL));
  ASSERT_NE(rights1.__value, rights2.__value);

  // Sets should now be identical.
  ASSERT_EQ(&rights2, cap_rights_set(&rights2, CAP_FCNTL));
  ASSERT_EQ(rights1.__value, rights2.__value);
}
