// Copyright (c) 2016 Nuxi, https://nuxi.nl/
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

#ifndef ___ARCH_ARM_SETJMP_H_
#define ___ARCH_ARM_SETJMP_H_

#include <_/types.h>

// Registers that need to be preserved across function calls, per the
// Procedure Call Standard for the ARM Architecture, section 5.1.1,
// table 2, and section 5.1.2.
struct __jmp_buf {
  __uint64_t __d8;   // Callee-saved VFP register.
  __uint64_t __d9;   // Callee-saved VFP register.
  __uint64_t __d10;  // Callee-saved VFP register.
  __uint64_t __d11;  // Callee-saved VFP register.
  __uint64_t __d12;  // Callee-saved VFP register.
  __uint64_t __d13;  // Callee-saved VFP register.
  __uint64_t __d14;  // Callee-saved VFP register.
  __uint64_t __d15;  // Callee-saved VFP register.

  __uint32_t __r4;  // Callee-saved register.
  __uint32_t __r5;  // Callee-saved register.
  __uint32_t __r6;  // Callee-saved register.
  __uint32_t __r7;  // Callee-saved register.
  __uint32_t __r8;  // Callee-saved register.
  __uint32_t __r9;  // Callee-saved register.
  __uint32_t __r10;  // Callee-saved register.
  __uint32_t __r11;  // Callee-saved register.
  __uint32_t __r12;  // The Intra-Procedure-call scratch register.
  __uint32_t __r13;  // The Stack Pointer.
  __uint32_t __r14;  // The Link Register.

};
_Static_assert(sizeof(struct __jmp_buf) == 112, "ABI breakage");

#endif
