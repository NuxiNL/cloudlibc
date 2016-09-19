// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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
#elif defined(__arm__)
  // Save VFP registers.
  vstmia r0, {d8-d15}
  add r0, r0, #64

  // Save callee-saved registers.
  stmia r0, {r4, r14}

  // Return zero for a direct invication.
  mov r0, #0
  bx lr
#elif defined(__i386__)
  // Save register values.
  mov 4(%esp), %ecx
  mov 0(%esp), %edx
  mov %edx, 0(%ecx)
  mov %ebx, 4(%ecx)
  mov %esp, 8(%ecx)
  mov %ebp, 12(%ecx)
  mov %esi, 16(%ecx)
  mov %edi, 20(%ecx)
  stmxcsr 24(%ecx)
  fnstcw 28(%ecx)

  // Return zero for a direct invocation.
  xor %eax, %eax
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
