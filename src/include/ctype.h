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

// <ctype.h> - character types
//
// Features missing:
// - *_l():
//   Would only be useful for 8-bit character sets.

#ifndef _CTYPE_H_
#define _CTYPE_H_

#include <_/types.h>

#ifndef _LOCALE_T_DECLARED
typedef __locale_t locale_t;
#define _LOCALE_T_DECLARED
#endif

__BEGIN_DECLS
int isalnum(int) __pure2;
int isalpha(int) __pure2;
int isblank(int) __pure2;
int iscntrl(int) __pure2;
int isdigit(int) __pure2;
int isgraph(int) __pure2;
int islower(int) __pure2;
int isprint(int) __pure2;
int ispunct(int) __pure2;
int isspace(int) __pure2;
int isupper(int) __pure2;
int isxdigit(int) __pure2;
int tolower(int) __pure2;
int toupper(int) __pure2;
__END_DECLS

#endif
