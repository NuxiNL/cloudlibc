// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <setjmp.h>
#include <testing.h>

// Clang doesn't provide setjmp()/longjmp() support on some architectures.
#if !defined(__aarch64__) && !defined(__riscv)

TEST(sigsetjmp_siglongjmp, example) {
  sigjmp_buf jmp;
  while (sigsetjmp(jmp, 0) != 123)
    siglongjmp(jmp, 123);
}

#endif
