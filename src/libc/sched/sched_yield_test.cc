// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sched.h>

#include "gtest/gtest.h"

TEST(sched_yield, example) {
  ASSERT_EQ(0, sched_yield());
}
