// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int fegetexceptflag(fexcept_t *flagp, int excepts) {
#if defined(__aarch64__)
  // Store exception flags in object.
  flagp->__exceptions = mrs_fpsr() & excepts;
  return 0;
#elif defined(__arm__)
  // Store exception flags in object.
  flagp->__exceptions = vmrs_fpscr() & excepts;
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
  // Combine the x87 and SSE exception flags.
  flagp->__exceptions = (fnstsw() | stmxcsr()) & excepts;
  return 0;
#else
#error "Unsupported platform"
#endif
}
