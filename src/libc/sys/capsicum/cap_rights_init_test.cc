// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include "gtest/gtest.h"

TEST(cap_rights_init, example) {
  cap_rights_t rights;
  ASSERT_EQ(&rights, cap_rights_init(&rights));
  ASSERT_EQ(0, rights.__value);
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_READ));
  ASSERT_EQ(CAP_READ, rights.__value);
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_READ, CAP_WRITE));
  ASSERT_EQ(CAP_READ | CAP_WRITE, rights.__value);
}
