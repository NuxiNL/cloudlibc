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

#ifndef ___ARCH_AARCH64_SETJMP_H_
#define ___ARCH_AARCH64_SETJMP_H_

#include <_/types.h>

// Registers that need to be preserved across function calls, per the
// Procedure Call Standard for the ARM 64-bit Architecture, section
// 5.1.1, table 2, and section 5.1.2.
struct __jmp_buf {
  __uint64_t __sp;  // The Stack Pointer.

  __uint64_t __x19;  // Callee-saved register.
  __uint64_t __x20;  // Callee-saved register.
  __uint64_t __x21;  // Callee-saved register.
  __uint64_t __x22;  // Callee-saved register.
  __uint64_t __x23;  // Callee-saved register.
  __uint64_t __x24;  // Callee-saved register.
  __uint64_t __x25;  // Callee-saved register.
  __uint64_t __x26;  // Callee-saved register.
  __uint64_t __x27;  // Callee-saved register.
  __uint64_t __x28;  // Callee-saved register.
  __uint64_t __x29;  // The Frame Pointer.
  __uint64_t __x30;  // The Link Register.

  __uint64_t __d8;   // Callee-saved VFP register.
  __uint64_t __d9;   // Callee-saved VFP register.
  __uint64_t __d10;  // Callee-saved VFP register.
  __uint64_t __d11;  // Callee-saved VFP register.
  __uint64_t __d12;  // Callee-saved VFP register.
  __uint64_t __d13;  // Callee-saved VFP register.
  __uint64_t __d14;  // Callee-saved VFP register.
  __uint64_t __d15;  // Callee-saved VFP register.
};
_Static_assert(sizeof(struct __jmp_buf) == 168, "ABI breakage");

#endif
