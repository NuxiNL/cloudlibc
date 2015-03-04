// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fesetround(int round) {
  // Disallow invalid rounding modes.
  if ((round & ~ROUNDING_MASK) != 0)
    return -1;

  // Update x87 rounding mode. Rounding mode is shifted by three bits
  // compared to how the SSE rouding mode is stored.
  uint16_t cw = fnstcw();
  fldcw((cw & ~(ROUNDING_MASK >> 3)) | (round >> 3));

  // Update SSE rounding mode.
  uint32_t mxcsr = stmxcsr();
  ldmxcsr((mxcsr & ~ROUNDING_MASK) | round);

  return 0;
}
