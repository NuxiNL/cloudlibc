// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <testing.h>

TEST(cap_rights_merge, example) {
  // Create two sets.
  cap_rights_t rights1;
  ASSERT_EQ(&rights1, cap_rights_init(&rights1, CAP_READ, CAP_WRITE));
  cap_rights_t rights2;
  ASSERT_EQ(&rights2, cap_rights_init(&rights2, CAP_READ, CAP_SHUTDOWN));

  // Merge them together.
  ASSERT_EQ(&rights1, cap_rights_merge(&rights1, &rights2));
  ASSERT_EQ(CAP_READ | CAP_WRITE | CAP_SHUTDOWN, rights1.__value);
}
