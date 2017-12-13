// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/assembly.h>

#ifdef __aarch64__

ENTRY(__setjmp)
  // Save callee-saved registers.
  stp x19, x20, [x0, #0]
  stp x21, x22, [x0, #16]
  stp x23, x24, [x0, #32]
  stp x25, x26, [x0, #48]
  stp x27, x28, [x0, #64]

  // Save frame pointer and link register.
  stp x29, x30, [x0, #80]

  // Save stack pointer.
  mov x1, sp
  str x1, [x0, #96]

  // Save VFP registers.
  stp d8, d9, [x0, #104]
  stp d10, d11, [x0, #120]
  stp d12, d13, [x0, #136]
  stp d14, d15, [x0, #152]

  // Return zero for a direct invocation.
  mov x0, #0
  ret
END(setjmp)

#endif
