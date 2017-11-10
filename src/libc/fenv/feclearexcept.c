// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int feclearexcept(int excepts) {
#if defined(__aarch64__)
  // Clear exceptions in status register.
  msr_fpsr(mrs_fpsr() & ~excepts);
  return 0;
#elif defined(__arm__)
  // Clear exceptions in status register.
  vmsr_fpscr(vmrs_fpscr() & ~excepts);
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
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
