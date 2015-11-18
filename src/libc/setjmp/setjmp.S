// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/assembly.h>

ENTRY(setjmp)
ALTENTRY(sigsetjmp)
#if defined(__aarch64__)
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
#elif defined(__x86_64__)
  // Save register values.
  mov (%rsp), %rdx
  mov %rdx, 0(%rdi)
  mov %rbx, 8(%rdi)
  mov %rsp, 16(%rdi)
  mov %rbp, 24(%rdi)
  mov %r12, 32(%rdi)
  mov %r13, 40(%rdi)
  mov %r14, 48(%rdi)
  mov %r15, 56(%rdi)
  stmxcsr 64(%rdi)
  fnstcw 68(%rdi)

  // Return zero for a direct invocation.
  xor %rax, %rax
  ret
#else
#error "Unsupported architecture"
#endif
END(setjmp)
END(sigsetjmp)
