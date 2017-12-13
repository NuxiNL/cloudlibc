// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <setjmp.h>

// Clang doesn't provide setjmp()/longjmp() support on some architectures.
#if !defined(__aarch64__) && !defined(__riscv)

void longjmp(jmp_buf env, int val) {
  env->__val = val == 0 ? 1 : val;
  __builtin_longjmp(env->__ptr, 1);
}

__strong_reference(longjmp, siglongjmp);

#endif
