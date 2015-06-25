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

// <wchar.h> - wide-character handling
//
// Extensions:
// - aswprintf(), aswprintf_l(), vaswprintf() and vaswprintf_l():
//   Analogous to asprintf().
// - dwprintf(), dwprintf_l(), vdwprintf() and vdwprintf_l():
//   Analogous to dprintf().
// - wcsfmon(), wcsfmon_l(), vwcsfmon() and vwcfsmon_l():
//   Analogous to strfmon(), strfmon_l(), vstrfmon() and vstrftmon_l().
// - wcslcat() and wcslcpy():
//   Analogous to strlcat() and strlcpy().
// - wcsndup():
//   Analogous to strndup().
// - wcsptime() and wcsptime_l():
//   Analogous to strptime() and strptime_l().
// - wmemmem():
//   Analogous to memmem().
// - *_l():
//   Regular functions always use the C locale.
//
// Features missing:
// - getwchar(), putwchar(), vprintf(), vscanf(), wprintf() and wscanf():
//   Standard descriptors are not available.
// - mbsinit():
//   Assumes encoding and decoding state share the same layout.
// - wcpcpy(), wcscat(), wcscpy() and wcsncat():
//   Prone to buffer overflows. Not safe to use.
// - ungetwc():
//   Bad design practice.
// - fwide() and open_wmemstream():
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
int aswprintf(wchar_t **__restrict, const wchar_t *__restrict, ...);
int aswprintf_l(wchar_t **__restrict, locale_t, const wchar_t *__restrict, ...);
wint_t btowc(int) __pure2;
wint_t btowc_l(int, locale_t) __pure;
int dwprintf(int, const wchar_t *__restrict, ...);
int dwprintf_l(int, locale_t, const wchar_t *__restrict, ...);
wint_t fgetwc(FILE *);
wchar_t *fgetws(wchar_t *__restrict, int, FILE *__restrict);
wint_t fputwc(wchar_t, FILE *);
int fputws(const wchar_t *__restrict, FILE *__restrict);
int fwprintf(FILE *__restrict, const wchar_t *__restrict, ...);
int fwprintf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, ...);
int fwscanf(FILE *__restrict, const wchar_t *__restrict, ...);
int fwscanf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, ...);
wint_t getwc(FILE *);
size_t mbrlen(const char *__restrict, size_t, mbstate_t *__restrict);
size_t mbrlen_l(const char *__restrict, size_t, mbstate_t *__restrict,
                locale_t);
size_t mbrtowc(wchar_t *__restrict, const char *__restrict, size_t,
               mbstate_t *__restrict);
size_t mbrtowc_l(wchar_t *__restrict, const char *__restrict, size_t,
                 mbstate_t *__restrict, locale_t);
size_t mbsnrtowcs(wchar_t *__restrict, const char **__restrict, size_t, size_t,
                  mbstate_t *__restrict);
size_t mbsnrtowcs_l(wchar_t *__restrict, const char **__restrict, size_t,
                    size_t, mbstate_t *__restrict, locale_t);
size_t mbsrtowcs(wchar_t *__restrict, const char **__restrict, size_t,
                 mbstate_t *__restrict);
size_t mbsrtowcs_l(wchar_t *__restrict, const char **__restrict, size_t,
                   mbstate_t *__restrict, locale_t);
wint_t putwc(wchar_t, FILE *);
int swprintf(wchar_t *__restrict, size_t, const wchar_t *__restrict, ...);
int swprintf_l(wchar_t *__restrict, size_t, locale_t, const wchar_t *__restrict,
               ...);
int swscanf(const wchar_t *__restrict, const wchar_t *__restrict, ...);
int swscanf_l(const wchar_t *__restrict, locale_t, const wchar_t *__restrict,
              ...);
int vaswprintf(wchar_t **__restrict, const wchar_t *__restrict, va_list);
int vaswprintf_l(wchar_t **__restrict, locale_t, const wchar_t *__restrict,
                 va_list);
int vdwprintf(int, const wchar_t *__restrict, va_list);
int vdwprintf_l(int, locale_t, const wchar_t *__restrict, va_list);
int vfwprintf(FILE *__restrict, const wchar_t *__restrict, va_list);
int vfwprintf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, va_list);
int vfwscanf(FILE *__restrict, const wchar_t *__restrict, va_list);
int vfwscanf_l(FILE *__restrict, locale_t, const wchar_t *__restrict, va_list);
int vswprintf(wchar_t *__restrict, size_t, const wchar_t *__restrict, va_list);
int vswprintf_l(wchar_t *__restrict, size_t, locale_t,
                const wchar_t *__restrict, va_list);
int vswscanf(const wchar_t *__restrict, const wchar_t *__restrict, va_list);
int vswscanf_l(const wchar_t *__restrict, locale_t, const wchar_t *__restrict,
               va_list);
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

#endif
