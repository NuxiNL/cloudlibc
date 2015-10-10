// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fegetround(void) {
#ifdef __x86_64__
  // Obtain rounding mode from SSE.
  return stmxcsr() & ROUNDING_MASK;
#else
#error "Unsupported platform"
#endif
}
