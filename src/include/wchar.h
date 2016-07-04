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

// <wchar.h> - wide-character handling
//
// Extensions:
// - aswprintf(), aswprintf_l(), vaswprintf() and vaswprintf_l():
//   Analogous to asprintf().
// - dwprintf(), dwprintf_l(), vdwprintf() and vdwprintf_l():
//   Analogous to dprintf().
// - getwdelim() and getwline():
//   Analogous to getdelim() and getline().
// - wcsfmon(), wcsfmon_l(), vwcsfmon() and vwcsfmon_l():
//   Analogous to strfmon(), strfmon_l(), vstrfmon() and vstrfmon_l().
// - wcslcat() and wcslcpy():
//   Analogous to strlcat() and strlcpy().
// - wcsndup():
//   Analogous to strndup().
// - wcsptime() and wcsptime_l():
//   Analogous to strptime() and strptime_l().
// - wcsverscmp():
//   Analogous to strverscmp().
// - wmemmem():
//   Analogous to memmem().
// - *_l():
//   Regular functions always use the C locale.
//
// Features missing:
// - getwchar(), putwchar(), vprintf(), vscanf(), wprintf() and wscanf():
//   Standard descriptors are not available.
// - open_wmemstream():
//   Streams are currently byte-oriented.

#ifndef _WCHAR_H_
#define _WCHAR_H_

#include <_/limits.h>
#include <_/types.h>

#ifndef _FILE_DECLARED
typedef struct _FILE FILE;
#define _FILE_DECLARED
#endif
#ifndef _LOCALE_T_DECLARED
typedef __locale_t locale_t;
#define _LOCALE_T_DECLARED
#endif
#ifndef _MBSTATE_T_DECLARED
typedef __mbstate_t mbstate_t;
#define _MBSTATE_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _VA_LIST_DECLARED
typedef __va_list va_list;
#define _VA_LIST_DECLARED
#endif
#ifndef _WCHAR_T_DECLARED
typedef __wchar_t wchar_t;
#define _WCHAR_T_DECLARED
#endif
#ifndef _WINT_T_DECLARED
typedef __wint_t wint_t;
#define _WINT_T_DECLARED
#endif

struct tm;

#define WCHAR_MIN _WCHAR_MIN
#define WCHAR_MAX _WCHAR_MAX

#define WEOF ((wint_t)-1)

#define NULL _NULL

__BEGIN_DECLS
int aswprintf(wchar_t **__restrict, const wchar_t *__restrict, ...)
    __wprintflike(2, 3);
int aswprintf_l(wchar_t **__restrict, locale_t, const wchar_t *__restrict, ...)
    __wprintflike(3, 4);
wint_t btowc(int) __pure2;
wint_t btowc_l(int, locale_t) __pure;
int dwprintf(int, const wchar_t *__restrict, ...) __wprintflike(2, 3);
int dwprintf_l(int, locale_t, const wchar_t *__restrict, ...)
    __wprintflike(3, 4);
wint_t fgetwc(FILE *);
wchar_t *fgetws(wchar_t *__restrict, int, FILE *__restrict);
wint_t fputwc(wchar_t, FILE *);
int fputws(const wchar_t *__restrict, FILE *__restrict);
int fwide(FILE *, int);
int fwprintf(FILE *__restrict, const wchar_t *__restrict, ...)
    __wprintflike(2, 3);
int fwprintf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, ...)
    __wprintflike(3, 4);
int fwscanf(FILE *__restrict, const wchar_t *__restrict, ...)
    __wscanflike(2, 3);
int fwscanf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, ...)
    __wscanflike(3, 4);
wint_t getwc(FILE *);
__ssize_t getwdelim(wchar_t **__restrict, size_t *__restrict, wint_t,
                    FILE *__restrict);
__ssize_t getwline(wchar_t **__restrict, size_t *__restrict, FILE *__restrict);
size_t mbrlen(const char *__restrict, size_t, mbstate_t *__restrict);
size_t mbrlen_l(const char *__restrict, size_t, mbstate_t *__restrict,
                locale_t);
size_t mbrtowc(wchar_t *__restrict, const char *__restrict, size_t,
               mbstate_t *__restrict);
size_t mbrtowc_l(wchar_t *__restrict, const char *__restrict, size_t,
                 mbstate_t *__restrict, locale_t);
int mbsinit(const mbstate_t *);
size_t mbsnrtowcs(wchar_t *__restrict, const char **__restrict, size_t, size_t,
                  mbstate_t *__restrict);
size_t mbsnrtowcs_l(wchar_t *__restrict, const char **__restrict, size_t,
                    size_t, mbstate_t *__restrict, locale_t);
size_t mbsrtowcs(wchar_t *__restrict, const char **__restrict, size_t,
                 mbstate_t *__restrict);
size_t mbsrtowcs_l(wchar_t *__restrict, const char **__restrict, size_t,
                   mbstate_t *__restrict, locale_t);
wint_t putwc(wchar_t, FILE *);
int swprintf(wchar_t *__restrict, size_t, const wchar_t *__restrict, ...)
    __wprintflike(3, 4);
int swprintf_l(wchar_t *__restrict, size_t, locale_t, const wchar_t *__restrict,
               ...) __wprintflike(4, 5);
int swscanf(const wchar_t *__restrict, const wchar_t *__restrict, ...)
    __wscanflike(2, 3);
int swscanf_l(const wchar_t *__restrict, locale_t, const wchar_t *__restrict,
              ...) __wscanflike(3, 4);
wint_t ungetwc(wint_t, FILE *);
int vaswprintf(wchar_t **__restrict, const wchar_t *__restrict, va_list)
    __wprintflike(2, 0);
int vaswprintf_l(wchar_t **__restrict, locale_t, const wchar_t *__restrict,
                 va_list) __wprintflike(3, 0);
int vdwprintf(int, const wchar_t *__restrict, va_list) __wprintflike(2, 0);
int vdwprintf_l(int, locale_t, const wchar_t *__restrict, va_list)
    __wprintflike(3, 0);
int vfwprintf(FILE *__restrict, const wchar_t *__restrict, va_list)
    __wprintflike(2, 0);
int vfwprintf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, va_list)
    __wprintflike(3, 0);
int vfwscanf(FILE *__restrict, const wchar_t *__restrict, va_list)
    __wscanflike(2, 0);
int vfwscanf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, va_list)
    __wscanflike(3, 0);
int vswprintf(wchar_t *__restrict, size_t, const wchar_t *__restrict, va_list)
    __wprintflike(3, 0);
int vswprintf_l(wchar_t *__restrict, size_t, locale_t,
                const wchar_t *__restrict, va_list) __wprintflike(3, 0);
int vswscanf(const wchar_t *__restrict, const wchar_t *__restrict, va_list)
    __wscanflike(2, 0);
int vswscanf_l(const wchar_t *__restrict, locale_t, const wchar_t *__restrict,
               va_list) __wscanflike(3, 0);
__ssize_t vwcsfmon(wchar_t *__restrict, size_t, const wchar_t *__restrict,
                   va_list);
__ssize_t vwcsfmon_l(wchar_t *__restrict, size_t, locale_t,
                     const wchar_t *__restrict, va_list);
wchar_t *wcpncpy(wchar_t *__restrict, const wchar_t *__restrict, size_t);
size_t wcrtomb(char *__restrict, wchar_t, mbstate_t *__restrict);
size_t wcrtomb_l(char *__restrict, wchar_t, mbstate_t *__restrict, locale_t);
int wcscasecmp(const wchar_t *, const wchar_t *) __pure;
int wcscasecmp_l(const wchar_t *, const wchar_t *, locale_t) __pure;
wchar_t *wcschr(const wchar_t *, wchar_t) __pure;
int wcscmp(const wchar_t *, const wchar_t *) __pure;
int wcscoll(const wchar_t *, const wchar_t *) __pure;
int wcscoll_l(const wchar_t *, const wchar_t *, locale_t) __pure;
size_t wcscspn(const wchar_t *, const wchar_t *) __pure;
wchar_t *wcsdup(const wchar_t *) __malloc_like;
__ssize_t wcsfmon(wchar_t *__restrict, size_t, const wchar_t *__restrict, ...);
__ssize_t wcsfmon_l(wchar_t *__restrict, size_t, locale_t,
                    const wchar_t *__restrict, ...);
size_t wcsftime(wchar_t *__restrict, size_t, const wchar_t *__restrict,
                const struct tm *__restrict);
size_t wcsftime_l(wchar_t *__restrict, size_t, const wchar_t *__restrict,
                  const struct tm *__restrict, locale_t);
size_t wcslcat(wchar_t *, const wchar_t *, size_t);
size_t wcslcpy(wchar_t *, const wchar_t *, size_t);
size_t wcslen(const wchar_t *) __pure;
int wcsncasecmp(const wchar_t *, const wchar_t *, size_t) __pure;
int wcsncasecmp_l(const wchar_t *, const wchar_t *, size_t, locale_t) __pure;
int wcsncmp(const wchar_t *, const wchar_t *, size_t) __pure;
wchar_t *wcsncpy(wchar_t *__restrict, const wchar_t *__restrict, size_t);
wchar_t *wcsndup(const wchar_t *, size_t) __malloc_like;
size_t wcsnlen(const wchar_t *, size_t) __pure;
size_t wcsnrtombs(char *__restrict, const wchar_t **__restrict, size_t, size_t,
                  mbstate_t *__restrict);
size_t wcsnrtombs_l(char *__restrict, const wchar_t **__restrict, size_t,
                    size_t, mbstate_t *__restrict, locale_t);
wchar_t *wcspbrk(const wchar_t *, const wchar_t *) __pure;
wchar_t *wcsptime(const wchar_t *__restrict, const wchar_t *__restrict,
                  struct tm *__restrict);
wchar_t *wcsptime_l(const wchar_t *__restrict, const wchar_t *__restrict,
                    struct tm *__restrict, locale_t);
wchar_t *wcsrchr(const wchar_t *, wchar_t) __pure;
size_t wcsrtombs(char *__restrict, const wchar_t **__restrict, size_t,
                 mbstate_t *__restrict);
size_t wcsrtombs_l(char *__restrict, const wchar_t **__restrict, size_t,
                   mbstate_t *__restrict, locale_t);
size_t wcsspn(const wchar_t *, const wchar_t *) __pure;
wchar_t *wcsstr(const wchar_t *__restrict, const wchar_t *__restrict) __pure;
double wcstod(const wchar_t *__restrict, wchar_t **__restrict);
double wcstod_l(const wchar_t *__restrict, wchar_t **__restrict, locale_t);
float wcstof(const wchar_t *__restrict, wchar_t **__restrict);
float wcstof_l(const wchar_t *__restrict, wchar_t **__restrict, locale_t);
wchar_t *wcstok(wchar_t *__restrict, const wchar_t *__restrict,
                wchar_t **__restrict);
long wcstol(const wchar_t *__restrict, wchar_t **__restrict, int);
long double wcstold(const wchar_t *__restrict, wchar_t **__restrict);
long double wcstold_l(const wchar_t *__restrict, wchar_t **__restrict,
                      locale_t);
long long wcstoll(const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long wcstoul(const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long long wcstoull(const wchar_t *__restrict, wchar_t **__restrict,
                            int);
int wcsverscmp(const wchar_t *, const wchar_t *) __pure;
int wcswidth(const wchar_t *, size_t) __pure;
size_t wcsxfrm(wchar_t *__restrict, const wchar_t *__restrict, size_t);
size_t wcsxfrm_l(wchar_t *__restrict, const wchar_t *__restrict, size_t,
                 locale_t);
int wctob(wint_t) __pure2;
int wctob_l(wint_t, locale_t) __pure;
int wcwidth(wchar_t) __pure2;
wchar_t *wmemchr(const wchar_t *, wchar_t, size_t) __pure;
int wmemcmp(const wchar_t *, const wchar_t *, size_t) __pure;
wchar_t *wmemcpy(wchar_t *__restrict, const wchar_t *__restrict, size_t);
wchar_t *wmemmem(const wchar_t *, size_t, const wchar_t *, size_t) __pure;
wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
#define wcschr(ws, wc) __preserve_const(wchar_t, wcschr, ws, ws, wc)
#define wcspbrk(ws1, ws2) __preserve_const(wchar_t, wcspbrk, ws1, ws1, ws2)
#define wcsrchr(ws, wc) __preserve_const(wchar_t, wcsrchr, ws, ws, wc)
#define wcsstr(ws1, ws2) __preserve_const(wchar_t, wcsstr, ws1, ws1, ws2)
#define wmemchr(ws, wc, n) __preserve_const(wchar_t, wmemchr, ws, ws, wc, n)
#define wmemmem(ws1, ws1len, ws2, ws2len) \
  __preserve_const(wchar_t, wmemmem, ws1, ws1, ws1len, ws2, ws2len)
#endif

#endif

#if !defined(_WCHAR_H_UNSAFE_) && defined(_CLOUDLIBC_UNSAFE_STRING_FUNCTIONS)
#define _WCHAR_H_UNSAFE_

__BEGIN_DECLS
wchar_t *wcpcpy(wchar_t *__restrict, const wchar_t *__restrict)
    __extname("__wcpcpy");
wchar_t *wcscat(wchar_t *__restrict, const wchar_t *__restrict)
    __extname("__wcscat");
wchar_t *wcscpy(wchar_t *__restrict, const wchar_t *__restrict)
    __extname("__wcscpy");
wchar_t *wcsncat(wchar_t *__restrict, const wchar_t *__restrict, size_t)
    __extname("__wcsncat");
__END_DECLS

#endif
