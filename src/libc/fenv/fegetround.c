// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fegetround(void) {
#if defined(__aarch64__)
  // Obtain rounding mode from FPCR.
  return mrs_fpcr() & ROUNDING_MASK;
#elif defined(__arm__)
  // TODO(ed): Implement.
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
  // Obtain rounding mode from SSE.
  return stmxcsr() & ROUNDING_MASK;
#else
#error "Unsupported platform"
#endif
}
