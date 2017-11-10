// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int fegetenv(fenv_t *envp) {
#if defined(__aarch64__)
  // Store control and status register.
  envp->__fpcr = mrs_fpcr();
  envp->__fpsr = mrs_fpsr();
  return 0;
#elif defined(__arm__)
  // Store single status and control register.
  envp->__fpscr = vmrs_fpscr();
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
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
