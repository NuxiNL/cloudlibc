// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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

#ifndef ___ARCH_I386_FENV_H_
#define ___ARCH_I386_FENV_H_

#include <_/types.h>

// State returned by fnstenv.
struct __x87_state {
  __uint32_t __control;  // Control Word.
  __uint32_t __status;   // Status Word.
  __uint32_t __tag;      // Tag Word.
  __uint32_t __fpu_ipo;  // FPU Instruction Pointer Offset.
  __uint32_t __fpu_ips;  // FPU Instruction Pointer Selector.
  __uint32_t __fpu_dpo;  // FPU Data Pointer Offset.
  __uint32_t __fpu_dps;  // FPU Data Pointer Selector.
};

typedef struct {
  struct __x87_state __x87;
  __uint32_t __mxcsr;
} fenv_t;

typedef struct {
  __uint8_t __exceptions;
} fexcept_t;

// Exception flags stored in the mxcsr register.
#define FE_INVALID 0x01
#define FE_DENORMAL 0x02  // Extension. Name also used on other systems.
#define FE_DIVBYZERO 0x04
#define FE_OVERFLOW 0x08
#define FE_UNDERFLOW 0x10
#define FE_INEXACT 0x20

#define FE_ALL_EXCEPT                                                     \
  (FE_INVALID | FE_DENORMAL | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | \
   FE_INEXACT)

// Rounding modes stored in the mxcsr register.
#define FE_TONEAREST 0x0000
#define FE_DOWNWARD 0x2000
#define FE_UPWARD 0x4000
#define FE_TOWARDZERO 0x6000

#endif
