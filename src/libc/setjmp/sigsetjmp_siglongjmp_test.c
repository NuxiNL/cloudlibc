// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
