// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int fesetround(int round) {
#if defined(__aarch64__)
  // Disallow invalid rounding modes.
  if ((round & ~ROUNDING_MASK) != 0)
    return -1;

  // Update FPCR rounding mode.
  msr_fpcr((mrs_fpcr() & ~ROUNDING_MASK) | round);
  return 0;
#elif defined(__arm__)
  // Disallow invalid rounding modes.
  if ((round & ~ROUNDING_MASK) != 0)
    return -1;

  // Update FPSCR rounding mode.
  vmsr_fpscr((vmrs_fpscr() & ~ROUNDING_MASK) | round);
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
  // Disallow invalid rounding modes.
  if ((round & ~ROUNDING_MASK) != 0)
    return -1;

  // Update x87 rounding mode. Rounding mode is shifted by three bits
  // compared to how the SSE rouding mode is stored.
  fldcw((fnstcw() & ~(ROUNDING_MASK >> 3)) | (round >> 3));

  // Update SSE rounding mode.
  ldmxcsr((stmxcsr() & ~ROUNDING_MASK) | round);

  return 0;
#else
#error "Unsupported platform"
#endif
}
