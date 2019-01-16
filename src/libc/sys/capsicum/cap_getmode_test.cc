// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include "gtest/gtest.h"

TEST(cap_getmode, example) {
  unsigned int mode;
  ASSERT_EQ(0, cap_getmode(&mode));
  ASSERT_TRUE(mode);
}
