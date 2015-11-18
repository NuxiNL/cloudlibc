// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <setjmp.h>
#include <testing.h>

TEST(sigsetjmp_siglongjmp, example) {
  volatile int jumped = 0;
  sigjmp_buf jmp;
  int ret = sigsetjmp(jmp, 0);
  if (!jumped) {
    ASSERT_EQ(0, ret);
    jumped = 1;
    siglongjmp(jmp, 123);
    ASSERT_TRUE(0);
  } else {
    ASSERT_EQ(123, ret);
  }
}
