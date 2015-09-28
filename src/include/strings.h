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

// <strings.h> - string operations
//
// Extensions:
// - ffsl() and ffsll():
//   Present on most operating systems.

#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <_/types.h>

#ifndef _LOCALE_T_DECLARED
typedef __locale_t locale_t;
#define _LOCALE_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif

__BEGIN_DECLS
int ffs(int) __pure2;
int ffsl(long) __pure2;
int ffsll(long long) __pure2;
int strcasecmp(const char *, const char *) __pure;
int strcasecmp_l(const char *, const char *, locale_t) __pure;
int strncasecmp(const char *, const char *, size_t) __pure;
int strncasecmp_l(const char *, const char *, size_t, locale_t) __pure;
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline int __ffs(int __i) {
  return __builtin_ffs(__i);
}
#define ffs(i) __ffs(i)

static __inline int __ffsl(long __i) {
  return __builtin_ffsl(__i);
}
#define ffsl(i) __ffsl(i)

static __inline int __ffsll(long long __i) {
  return __builtin_ffsll(__i);
}
#define ffsll(i) __ffsll(i)
#endif

#endif
