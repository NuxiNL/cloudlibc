// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fesetenv(const fenv_t *envp) {
#if defined(__aarch64__)
  // Set control and status register.
  msr_fpcr(envp->__fpcr);
  msr_fpsr(envp->__fpsr);
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
  // Set both x87 and SSE state.
  fldenv(&envp->__x87);
  ldmxcsr(envp->__mxcsr);
  return 0;
#else
#error "Unsupported platform"
#endif
}
