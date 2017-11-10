// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int fegetround(void) {
#if defined(__aarch64__)
  // Obtain rounding mode from FPCR.
  return mrs_fpcr() & ROUNDING_MASK;
#elif defined(__arm__)
  // Obtain rounding mode from FPSCR.
  return vmrs_fpscr() & ROUNDING_MASK;
#elif defined(__i386__) || defined(__x86_64__)
  // Obtain rounding mode from SSE.
  return stmxcsr() & ROUNDING_MASK;
#else
#error "Unsupported platform"
#endif
}
