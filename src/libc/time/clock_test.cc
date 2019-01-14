// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(clock, example) {
  clock_t a = clock();
  clock_t b = clock();
  ASSERT_LE(a, b);
}
