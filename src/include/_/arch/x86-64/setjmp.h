// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifndef ___ARCH_X86_64_SETJMP_H_
#define ___ARCH_X86_64_SETJMP_H_

#include <_/types.h>

// Registers that need to be preserved across function calls, per
// Figure 3.4 of the AMD64 ABI.
struct __jmp_buf {
  __uint64_t __return;  // Return address.
  __uint64_t __rbx;     // %rbx.
  __uint64_t __rsp;     // %rsp.
  __uint64_t __rbp;     // %rbp.
  __uint64_t __r12;     // %r12.
  __uint64_t __r13;     // %r13.
  __uint64_t __r14;     // %r14.
  __uint64_t __r15;     // %r15.
  __uint32_t __mxcsr;   // mxcsr.
  __uint16_t __x87_cw;  // x87 control word.
};
_Static_assert(sizeof(struct __jmp_buf) == 72, "ABI breakage");

#endif
