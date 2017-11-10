// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int fetestexcept(int excepts) {
#if defined(__aarch64__)
  // Fetch exception flags.
  return mrs_fpsr() & excepts;
#elif defined(__arm__)
  // Fetch exception flags.
  return vmrs_fpscr() & excepts;
#elif defined(__i386__) || defined(__x86_64__)
  // Combine the x87 and SSE exception flags.
  return (fnstsw() | stmxcsr()) & excepts;
#else
#error "Unsupported platform"
#endif
}
