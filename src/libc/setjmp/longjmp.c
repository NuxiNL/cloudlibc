// Copyright (c) 2017-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <setjmp.h>
#include <cloudlibc_interceptors.h>

#ifndef __aarch64__

void __cloudlibc_longjmp(jmp_buf env, int val) {
  env->__val = val == 0 ? 1 : val;
  __builtin_longjmp(env->__ptr, 1);
}

__strong_reference(__cloudlibc_longjmp, __cloudlibc_siglongjmp);
__weak_reference(__cloudlibc_longjmp, longjmp);
__weak_reference(__cloudlibc_siglongjmp, siglongjmp);

#endif
