// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/assembly.h>

ENTRY(longjmp)
ALTENTRY(siglongjmp)
#if defined(__aarch64__)
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
#elif defined(__x86_64__)
  // Restore register values.
  mov 0(%rdi), %rdx
  mov %rdx, (%rsp)
  mov 8(%rdi), %rbx
  mov 16(%rdi), %rsp
  mov 24(%rdi), %rbp
  mov 32(%rdi), %r12
  mov 40(%rdi), %r13
  mov 48(%rdi), %r14
  mov 56(%rdi), %r15
  ldmxcsr 64(%rdi)
  fldcw 68(%rdi)

  // Return the second parameter of longjmp(), ensuring that we never
  // return zero.
  mov %rsi, %rax
  test %rax, %rax
  jnz 1f
  inc %rax
1:
  ret
#else
#error "Unsupported architecture"
#endif
END(longjmp)
END(siglongjmp)
