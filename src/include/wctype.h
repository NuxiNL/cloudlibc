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

// <wctype.h> - wide-character classification and mapping utilities

#ifndef _WCTYPE_H_
#define _WCTYPE_H_

#include <_/types.h>

#ifndef _LOCALE_T_DECLARED
typedef __locale_t locale_t;
#define _LOCALE_T_DECLARED
#endif
#ifndef _WINT_T_DECLARED
typedef __wint_t wint_t;
#define _WINT_T_DECLARED
#endif
typedef wint_t (*wctrans_t)(wint_t);
typedef int (*wctype_t)(wint_t);

#define WEOF ((wint_t)-1)

__BEGIN_DECLS
int iswalnum(wint_t) __pure2;
int iswalnum_l(wint_t, locale_t) __pure;
int iswalpha(wint_t) __pure2;
int iswalpha_l(wint_t, locale_t) __pure;
int iswblank(wint_t) __pure2;
int iswblank_l(wint_t, locale_t) __pure;
int iswcntrl(wint_t) __pure2;
int iswcntrl_l(wint_t, locale_t) __pure;
int iswctype(wint_t, wctype_t) __pure2;
int iswctype_l(wint_t, wctype_t, locale_t) __pure;
int iswdigit(wint_t) __pure2;
int iswdigit_l(wint_t, locale_t) __pure;
int iswgraph(wint_t) __pure2;
int iswgraph_l(wint_t, locale_t) __pure;
int iswlower(wint_t) __pure2;
int iswlower_l(wint_t, locale_t) __pure;
int iswprint(wint_t) __pure2;
int iswprint_l(wint_t, locale_t) __pure;
int iswpunct(wint_t) __pure2;
int iswpunct_l(wint_t, locale_t) __pure;
int iswspace(wint_t) __pure2;
int iswspace_l(wint_t, locale_t) __pure;
int iswupper(wint_t) __pure2;
int iswupper_l(wint_t, locale_t) __pure;
int iswxdigit(wint_t) __pure2;
int iswxdigit_l(wint_t, locale_t) __pure;
wint_t towctrans(wint_t, wctrans_t) __pure2;
wint_t towctrans_l(wint_t, wctrans_t, locale_t) __pure;
wint_t towlower(wint_t) __pure2;
wint_t towlower_l(wint_t, locale_t) __pure;
wint_t towupper(wint_t) __pure2;
wint_t towupper_l(wint_t, locale_t) __pure;
wctrans_t wctrans(const char *) __pure;
wctrans_t wctrans_l(const char *, locale_t) __pure;
wctype_t wctype(const char *) __pure;
wctype_t wctype_l(const char *, locale_t) __pure;
__END_DECLS

#endif
