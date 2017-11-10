// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "fenv_impl.h"

int feholdexcept(fenv_t *envp) {
#if defined(__aarch64__)
  // Save state and clear exceptions.
  envp->__fpcr = mrs_fpcr();
  envp->__fpsr = mrs_fpsr();
  msr_fpsr(envp->__fpsr & ~FE_ALL_EXCEPT);
  return 0;
#elif defined(__arm__)
  // Save state and clear exceptions.
  envp->__fpscr = vmrs_fpscr();
  vmsr_fpscr(envp->__fpscr & ~FE_ALL_EXCEPT);
  return 0;
#elif defined(__i386__) || defined(__x86_64__)
  // Save x87 and SSE state.
  fnstenv(&envp->__x87);
  envp->__mxcsr = stmxcsr();

  // Clear x87 floating-point exceptions. There is no need to mask
  // floating-point exceptions, as this is already done by fnstenv().
  fnclex();

  // Clear SSE floating-point exceptions. Also mask all exceptions.
  uint32_t mxcsr = envp->__mxcsr;
  mxcsr &= ~FE_ALL_EXCEPT;
  mxcsr |= FE_ALL_EXCEPT << EXCEPTION_MASK_SHIFT;
  ldmxcsr(mxcsr);

  return 0;
#else
#error "Unsupported platform"
#endif
}
