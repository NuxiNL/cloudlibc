// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <execinfo.h>
#include <testing.h>

TEST(backtrace, example) {
  void *trace[10];
  ASSERT_EQ(0, backtrace(trace, 0));
  ASSERT_EQ(1, backtrace(trace, 1));
  ASSERT_LE(1, backtrace(trace, __arraycount(trace)));
}
