// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fesetexceptflag(const fexcept_t *flagp, int excepts) {
#if defined(__aarch64__)
  // Set exceptions stored in object.
  msr_fpsr((mrs_fpsr() & ~excepts) | (flagp->__exceptions & excepts));
  return 0;
#elif defined(__x86_64__)
  // Set x87 exceptions.
  struct __x87_state x87_state;
  fnstenv(&x87_state);
  x87_state.__status &= ~excepts;
  x87_state.__status |= flagp->__exceptions & excepts;
  fldenv(&x87_state);

  // Set SSE exceptions.
  ldmxcsr((stmxcsr() & ~excepts) | (flagp->__exceptions & excepts));

  return 0;
#else
#error "Unsupported platform"
#endif
}
