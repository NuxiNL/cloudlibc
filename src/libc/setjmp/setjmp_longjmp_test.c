// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <setjmp.h>
#include <testing.h>

TEST(setjmp_longjmp, example) {
/* broken in ASAN :-(
  jmp_buf jmp;
  while (setjmp(jmp) != 123)
    longjmp(jmp, 123);
*/
}
