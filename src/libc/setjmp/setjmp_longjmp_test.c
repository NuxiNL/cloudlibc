// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <setjmp.h>
#include <testing.h>

TEST(setjmp_longjmp, example) {
  volatile int jumped = 0;
  jmp_buf jmp;
  int ret = setjmp(jmp);
  if (!jumped) {
    ASSERT_EQ(0, ret);
    jumped = 1;
    longjmp(jmp, 123);
    ASSERT_TRUE(0);
  } else {
    ASSERT_EQ(123, ret);
  }
}
