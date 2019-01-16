// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include "gtest/gtest.h"

TEST(cap_rights_remove, example) {
  // Create two sets.
  cap_rights_t rights1;
  ASSERT_EQ(&rights1, cap_rights_init(&rights1, CAP_READ, CAP_WRITE));
  cap_rights_t rights2;
  ASSERT_EQ(&rights2, cap_rights_init(&rights2, CAP_READ, CAP_SHUTDOWN));

  // Remove rights in the second set from the first.
  ASSERT_EQ(&rights1, cap_rights_remove(&rights1, &rights2));
  ASSERT_EQ(CAP_WRITE, rights1.__value);
}
