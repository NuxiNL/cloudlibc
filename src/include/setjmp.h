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

// <setjmp.h> - stack environment declarations

#ifndef _SETJMP_H_
#define _SETJMP_H_

#if defined(__aarch64__)
#include <_/arch/aarch64/setjmp.h>
#elif defined(__arm__)
#include <_/arch/arm/setjmp.h>
#elif defined(__i386__)
#include <_/arch/i386/setjmp.h>
#elif defined(__x86_64__)
#include <_/arch/x86_64/setjmp.h>
#else
#error "Unknown architecture"
#endif
#include <_/cdefs.h>

typedef struct {
  struct __jmp_buf __jmp_buf;
} jmp_buf[1];
typedef struct {
  struct __jmp_buf __jmp_buf;
} sigjmp_buf[1];

__BEGIN_DECLS
_Noreturn void longjmp(jmp_buf, int);
_Noreturn void siglongjmp(sigjmp_buf, int);
int setjmp(jmp_buf);
int sigsetjmp(sigjmp_buf, int);
__END_DECLS

#endif
