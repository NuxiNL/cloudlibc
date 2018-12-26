// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <setjmp.h>
#include <testing.h>

TEST(sigsetjmp_siglongjmp, example) {
/* broken in ASAN :-(
  sigjmp_buf jmp;
  while (sigsetjmp(jmp, 0) != 123)
    siglongjmp(jmp, 123);
*/
}
