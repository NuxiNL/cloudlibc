// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int feclearexcept(int excepts) {
#ifdef __x86_64__
  // Clear x87 exceptions.
  struct __x87_state x87_state;
  fnstenv(&x87_state);
  x87_state.__status &= ~excepts;
  fldenv(&x87_state);

  // Clear SSE exceptions.
  uint32_t mxcsr = stmxcsr();
  ldmxcsr(mxcsr & ~excepts);

  return 0;
#else
#error "Unsupported platform"
#endif
}
