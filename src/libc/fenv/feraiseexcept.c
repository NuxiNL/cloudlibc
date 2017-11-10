// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int feraiseexcept(int excepts) {
#if defined(__aarch64__)
  // Raise floating point exceptions.
  msr_fpsr(mrs_fpsr() | excepts);
  return 0;
#elif defined(__arm__)
  // Raise floating point exceptions.
  vmsr_fpscr(vmrs_fpscr() | excepts);
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
  // Set x87 exceptions.
  struct __x87_state x87_state;
  fnstenv(&x87_state);
  x87_state.__status |= excepts;
  fldenv(&x87_state);

  // Set SSE exceptions.
  ldmxcsr(stmxcsr() | excepts);

  // Wait for floating-point unit changes to be applied.
  fwait();
  return 0;
#else
#error "Unsupported platform"
#endif
}
