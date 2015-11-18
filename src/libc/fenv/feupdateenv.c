// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int feupdateenv(const fenv_t *envp) {
#if defined(__aarch64__)
  // Restore environment, while raising existing exceptions.
  msr_fpcr(envp->__fpcr);
  msr_fpsr(envp->__fpsr | (mrs_fpsr() & FE_ALL_EXCEPT));
  return 0;
#elif defined(__x86_64__)
  // Save currently raised exceptions.
  int exceptions = (fnstsw() | stmxcsr()) & FE_ALL_EXCEPT;

  // Set both x87 and SSE state.
  fldenv(&envp->__x87);
  ldmxcsr(envp->__mxcsr);

  // Raise saved exceptions.
  return feraiseexcept(exceptions);
#else
#error "Unsupported platform"
#endif
}
