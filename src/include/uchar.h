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

// <uchar.h> - Unicode utilities
//
// Extensions:
// - *_l():
//   Regular functions always use the C locale.

#ifndef _UCHAR_H_
#define _UCHAR_H_

#include <_/types.h>

#ifndef _CHAR16_T_DECLARED
typedef __char16_t char16_t;
#define _CHAR16_T_DECLARED
#endif
#ifndef _CHAR32_T_DECLARED
typedef __char32_t char32_t;
#define _CHAR32_T_DECLARED
#endif
#ifndef _MBSTATE_T_DECLARED
typedef __mbstate_t mbstate_t;
#define _MBSTATE_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif

__BEGIN_DECLS
size_t c16rtomb(char *__restrict, char16_t, mbstate_t *__restrict);
size_t c16rtomb_l(char *__restrict, char16_t, mbstate_t *__restrict,
                  __locale_t);
size_t c32rtomb(char *__restrict, char32_t, mbstate_t *__restrict);
size_t c32rtomb_l(char *__restrict, char32_t, mbstate_t *__restrict,
                  __locale_t);
size_t mbrtoc16(char16_t *__restrict, const char *__restrict, size_t,
                mbstate_t *__restrict);
size_t mbrtoc16_l(char16_t *__restrict, const char *__restrict, size_t,
                  mbstate_t *__restrict, __locale_t);
size_t mbrtoc32(char32_t *__restrict, const char *__restrict, size_t,
                mbstate_t *__restrict);
size_t mbrtoc32_l(char32_t *__restrict, const char *__restrict, size_t,
                  mbstate_t *__restrict, __locale_t);
__END_DECLS

#endif
