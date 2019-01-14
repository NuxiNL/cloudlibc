// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(daylight, example) {
  // The global timezone in this environment is always UTC.
  ASSERT_EQ(0, daylight);
}
