// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
