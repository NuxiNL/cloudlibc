// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <execinfo.h>
#include <testing.h>

TEST(backtrace, example) {
  void *trace[10];
  ASSERT_EQ(0, backtrace(trace, 0));
  ASSERT_EQ(1, backtrace(trace, 1));
  ASSERT_LE(1, backtrace(trace, __arraycount(trace)));
}
