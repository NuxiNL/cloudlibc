// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/assembly.h>

ENTRY(longjmp)
ALTENTRY(siglongjmp)
#if defined(__x86_64__)
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
