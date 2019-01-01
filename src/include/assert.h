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

// <assert.h> - verify program assertion

#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <_/types.h>

#ifndef __cplusplus
#define static_assert _Static_assert
#endif

__BEGIN_DECLS
_Noreturn void __assertion_failed(const char *, const char *, int,
                                  const char *);
__END_DECLS

static __inline void __assert_disabled(void) {
}

static __inline void __assert_enabled(const char *__func, const char *__file,
                                      int __line, _Bool __expression,
                                      const char *__expression_string) {
  if (!__expression)
    __assertion_failed(__func, __file, __line, __expression_string);
}

#endif

#undef assert
#ifdef NDEBUG
#define assert(expression) __assert_disabled()
#else
#define assert(expression) \
  __assert_enabled(__func__, __FILE__, __LINE__, !!(expression), #expression)
#endif
