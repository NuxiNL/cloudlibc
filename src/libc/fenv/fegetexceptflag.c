// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fegetexceptflag(fexcept_t *flagp, int excepts) {
#if defined(__aarch64__)
  // Store exception flags in object.
  flagp->__exceptions = mrs_fpsr() & excepts;
  return 0;
#elif defined(__x86_64__)
  // Combine the x87 and SSE exception flags.
  flagp->__exceptions = (fnstsw() | stmxcsr()) & excepts;
  return 0;
#else
#error "Unsupported platform"
#endif
}