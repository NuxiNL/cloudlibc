// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(atoll, examples) {
  ASSERT_EQ(5050505, atoll(" 5050505 "));
}
