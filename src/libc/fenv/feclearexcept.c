// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int feclearexcept(int excepts) {
#if defined(__aarch64__)
  // Clear exceptions in status register.
  msr_fpsr(mrs_fpsr() & ~excepts);
  return 0;
#elif defined(__x86_64__)
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
