// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/assembly.h>

#ifdef __aarch64__

ENTRY(longjmp)
ALTENTRY(siglongjmp)
  // Restore callee-saved registers.
  ldp x19, x20, [x0, #0]
  ldp x21, x22, [x0, #16]
  ldp x23, x24, [x0, #32]
  ldp x25, x26, [x0, #48]
  ldp x27, x28, [x0, #64]

  // Restore frame pointer and link register.
  ldp x29, x30, [x0, #80]

  // Restore stack pointer.
  ldr x2, [x0, #96]
  mov sp, x2

  // Restore VFP registers.
  ldp d8, d9, [x0, #104]
  ldp d10, d11, [x0, #120]
  ldp d12, d13, [x0, #136]
  ldp d14, d15, [x0, #152]

  // Return the second parameter of longjmp(), ensuring that we never
  // return zero.
  mov x0, x1
  cbnz x0, 1f
  mov x0, #1
1:
  br x30
END(longjmp)
END(siglongjmp)

#endif
