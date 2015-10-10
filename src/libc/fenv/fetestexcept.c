// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fetestexcept(int excepts) {
#ifdef __x86_64__
  // Combine the x87 and SSE exception flags.
  return (fnstsw() | stmxcsr()) & excepts;
#else
#error "Unsupported platform"
#endif
}
