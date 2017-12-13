// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
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

#include <_/types.h>

#ifdef __aarch64__

// The compiler doesn't support __builtin_longjmp() and __builtin_setjmp().
// Implement longjmp() and setjmp() as C library functions. These are
// less efficient.

typedef __uint64_t jmp_buf[21];

__BEGIN_DECLS
int setjmp(jmp_buf) __extname("__setjmp");
__END_DECLS

#else

// The compiler supports __builtin_longjmp() and __builtin_setjmp().
// Implement longjmp() and setjmp() on top of these intrinsics.

typedef struct __jmp_buf {
  int __val;
  void *__ptr[5];
} jmp_buf[1];

#define setjmp(env)                  \
  ({                                 \
    struct __jmp_buf *__env = (env); \
    __env->__val = 0;                \
    __builtin_setjmp(__env->__ptr);  \
    __env->__val;                    \
  })

#endif

typedef jmp_buf sigjmp_buf;
#define sigsetjmp(env, savemask) setjmp(env)

__BEGIN_DECLS
_Noreturn void longjmp(jmp_buf, int);
_Noreturn void siglongjmp(sigjmp_buf, int);
__END_DECLS

#endif
