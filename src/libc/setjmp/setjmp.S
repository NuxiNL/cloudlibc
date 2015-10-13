// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/assembly.h>

ENTRY(setjmp)
ALTENTRY(sigsetjmp)
#if defined(__x86_64__)
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
