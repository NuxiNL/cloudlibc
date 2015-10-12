// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

#include "fenv_impl.h"

int fegetenv(fenv_t *envp) {
#if defined(__aarch64__)
  // Store control and status register.
  envp->__fpcr = mrs_fpcr();
  envp->__fpsr = mrs_fpsr();
  return 0;
#elif defined(__x86_64__)
  // Save x87 state. As fnstenv also has the side-effect of masking all
  // future exceptions, we need to restore the control word manually.
  fnstenv(&envp->__x87);
  fldcw(envp->__x87.__control);

  // Save SSE state.
  envp->__mxcsr = stmxcsr();

  return 0;
#else
#error "Unsupported platform"
#endif
}
