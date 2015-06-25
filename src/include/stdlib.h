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

// <stdlib.h> - standard library definitions
//
// Extensions:
// - MB_CUR_MAX_L():
//   Replacement for MB_CUR_MAX. Available on many other operating
//   systems.
// - alloca():
//   Present on most other operating systems.
// - arc4random(), arc4random_buf() and arc4random_uniform():
//   Secure random number generator. Available on many other operating
//   systems.
// - mbstowcs_l() and wcstombs_l():
//   mbstowcs() and wcstombs() always use the C locale.
// - reallocarray():
//   Allows for reallocation of buffers without integer overflows.
//
// Features missing:
// - erand48(), jrand48() and nrand48():
//   Quality of random numbers is too weak. Use arc4random*() instead.
// - initstate(), lcong48(), setstate(), srand(), srand48() and srandom():
//   Randomizer is seeded securely by default. There is no need to seed
//   manually.
// - WEXITSTATUS(), WIFEXITED(), WIFSIGNALED(), WIFSTOPPED(),
//   WSTOPSIG(), WTERMSIG(), WUNTRACED:
//   Only useful if system() would actually work.
// - a64l() and l64a():
//   Not thread-safe.
// - putenv(), setenv() and unsetenv():
//   Environment variables are not available.
// - grantpt(), posix_openpt(), ptsname() and unlockpt():
//   Pseudo-terminals are not available.
// - mblen(), mbtowc(), wctomb():
//   Not thread-safe. Use mbrlen(), mbrtowc() and wcrtomb() instead.
// - mkdtemp(), mkstemp() and realpath():
//   Requires global filesystem namespace.
// - setkey():
//   Password database and encryption schemes not available.

#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <_/limits.h>
#include <_/types.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX _INT_MAX

#define NULL _NULL

typedef struct {
  int quot;
  int rem;
} div_t;

typedef struct {
  long quot;
  long rem;
} ldiv_t;

typedef struct {
  long long quot;
  long long rem;
} lldiv_t;

#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _WCHAR_T_DECLARED
typedef __wchar_t wchar_t;
#define _WCHAR_T_DECLARED
#endif

// Process wide locale always uses ASCII.
#define MB_CUR_MAX 1

// Keep existing code happy that assumes that MB_CUR_MAX_L is a macro.
#define MB_CUR_MAX_L MB_CUR_MAX_L

// Flags for pdwait().
#define WNOHANG 0x1

// Inline versions of trivial routines.

static __inline int __abs(int __i) {
  return __i < 0 ? -__i : __i;
}

static __inline long __labs(long __i) {
  return __i < 0 ? -__i : __i;
}

static __inline long long __llabs(long long __i) {
  return __i < 0 ? -__i : __i;
}

static __inline div_t __div(int __numer, int __denom) {
  div_t __res = {__numer / __denom, __numer % __denom};
  return __res;
}

static __inline ldiv_t __ldiv(long __numer, long __denom) {
  ldiv_t __res = {__numer / __denom, __numer % __denom};
  return __res;
}

static __inline lldiv_t __lldiv(long long __numer, long long __denom) {
  lldiv_t __res = {__numer / __denom, __numer % __denom};
  return __res;
}

static __inline void *__bsearch(const void *__key, const void *__base,
                                size_t __nel, size_t __width,
                                int (*__compar)(const void *, const void *)) {
  const char *__basep, *__obj;
  size_t __mid, __skip;
  int __cmp;

  __basep = (const char *)__base;
  while (__nel > 0) {
    // Pick pivot.
    __mid = __nel / 2;
    __obj = __basep + __mid * __width;
    __cmp = __compar(__key, (const void *)__obj);
    if (__cmp < 0) {
      // key < obj. Restrict search to top of the list.
      __nel = __mid;
    } else if (__cmp > 0) {
      // key > obj. Restrict search to bottom of the list.
      __skip = __mid + 1;
      __basep += __skip * __width;
      __nel -= __skip;
    } else {
      return (void *)__obj;
    }
  }
  return NULL;
}

// qsort() implementation from Bentley and McIlroy's
// "Engineering a Sort Function".
//
// This sorting function is inlined into this header, so that the
// compiler can create an optimized version that takes the alignment and
// size of the elements into account. It also reduces the overhead of
// indirect function calls.

static __inline size_t __qsort_min(size_t __a, size_t __b) {
  return __a < __b ? __a : __b;
}

static __inline void __qsort_swap(char *__a, char *__b, size_t __n) {
  char t;

  while (__n-- > 0) {
    t = *__a;
    *__a++ = *__b;
    *__b++ = t;
  }
}

static __inline char *__qsort_med3(char *__a, char *__b, char *__c,
                                   int (*__cmp)(const void *, const void *)) {
  return __cmp(__a, __b) < 0
             ? (__cmp(__b, __c) < 0 ? __b : __cmp(__a, __c) < 0 ? __c : __a)
             : (__cmp(__b, __c) > 0 ? __b : __cmp(__a, __c) > 0 ? __c : __a);
}

static __inline void __qsort(void *__base, size_t __nel, size_t __width,
                             int (*__cmp)(const void *, const void *)) {
  char *__a, *__pa, *__pb, *__pc, *__pd, *__pl, *__pm, *__pn, *__pv;
  int __r;
  size_t __s;

  __a = (char *)__base;
  if (__nel < 7) {
    for (__pm = __a + __width; __pm < __a + __nel * __width; __pm += __width)
      for (__pl = __pm; __pl > __a && __cmp(__pl - __width, __pl) > 0;
           __pl -= __width)
        __qsort_swap(__pl, __pl - __width, __width);
    return;
  }
  __pm = __a + (__nel / 2) * __width;
  if (__nel > 7) {
    __pl = __a;
    __pn = __a + (__nel - 1) * __width;
    if (__nel > 40) {
      __s = (__nel / 8) * __width;
      __pl = __qsort_med3(__pl, __pl + __s, __pl + 2 * __s, __cmp);
      __pm = __qsort_med3(__pm - __s, __pm, __pm + __s, __cmp);
      __pn = __qsort_med3(__pn - 2 * __s, __pn - __s, __pn, __cmp);
    }
    __pm = __qsort_med3(__pl, __pm, __pn, __cmp);
  }
  __pv = __a;
  __qsort_swap(__pv, __pm, __width);
  __pa = __pb = __a;
  __pc = __pd = __a + (__nel - 1) * __width;
  for (;;) {
    while (__pb <= __pc && (__r = __cmp(__pb, __pv)) <= 0) {
      if (__r == 0) {
        __qsort_swap(__pa, __pb, __width);
        __pa += __width;
      }
      __pb += __width;
    }
    while (__pc >= __pb && (__r = __cmp(__pc, __pv)) >= 0) {
      if (__r == 0) {
        __qsort_swap(__pc, __pd, __width);
        __pd -= __width;
      }
      __pc -= __width;
    }
    if (__pb > __pc)
      break;
    __qsort_swap(__pb, __pc, __width);
    __pb += __width;
    __pc -= __width;
  }
  __pn = __a + __nel * __width;
  __s = __qsort_min(__pa - __a, __pb - __pa);
  if (__s > 0)
    __qsort_swap(__a, __pb - __s, __s);
  __s = __qsort_min(__pd - __pc, __pn - __pd - __width);
  if (__s > 0)
    __qsort_swap(__pb, __pn - __s, __s);
  __s = __pb - __pa;
  if (__s > __width)
    __qsort(__a, __s / __width, __width, __cmp);
  __s = __pd - __pc;
  if (__s > __width)
    __qsort(__pn - __s, __s / __width, __width, __cmp);
}

__BEGIN_DECLS
_Noreturn void _Exit(int);
size_t MB_CUR_MAX_L(__locale_t);
_Noreturn void abort(void);
int abs(int) __pure2;
int at_quick_exit(void (*)(void));
int atexit(void (*)(void));
void *aligned_alloc(size_t, size_t);
__uint32_t arc4random(void);
void arc4random_buf(void *, size_t);
__uint32_t arc4random_uniform(__uint32_t);
double atof(const char *);
int atoi(const char *);
long atol(const char *);
long long atoll(const char *);
void *bsearch(const void *, const void *, size_t, size_t,
              int (*)(const void *, const void *));
void *calloc(size_t, size_t);
div_t div(int, int) __pure2;
double drand48(void);
void exit(int);
void free(void *);
char *getenv(const char *);
int getsubopt(char **, char *const *, char **);
long labs(long) __pure2;
ldiv_t ldiv(long, long) __pure2;
long long llabs(long long) __pure2;
lldiv_t lldiv(long long, long long) __pure2;
long lrand48(void);
void *malloc(size_t);
size_t mbstowcs(wchar_t *__restrict, const char *__restrict, size_t);
size_t mbstowcs_l(wchar_t *__restrict, const char *__restrict, size_t,
                  __locale_t);
long mrand48(void);
int posix_memalign(void **, size_t, size_t);
void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
_Noreturn void quick_exit(int);
int rand(void);
long random(void);
void *realloc(void *, size_t);
void *reallocarray(void *, size_t, size_t);
double strtod(const char *__restrict, char **__restrict);
double strtod_l(const char *__restrict, char **__restrict, __locale_t);
float strtof(const char *__restrict, char **__restrict);
float strtof_l(const char *__restrict, char **__restrict, __locale_t);
long strtol(const char *__restrict, char **__restrict, int);
long strtol_l(const char *__restrict, char **__restrict, int, __locale_t);
long double strtold(const char *__restrict, char **__restrict);
long double strtold_l(const char *__restrict, char **__restrict, __locale_t);
long long strtoll(const char *__restrict, char **__restrict, int);
long long strtoll_l(const char *__restrict, char **__restrict, int, __locale_t);
unsigned long strtoul(const char *__restrict, char **__restrict, int);
unsigned long strtoul_l(const char *__restrict, char **__restrict, int,
                        __locale_t);
unsigned long long strtoull(const char *__restrict, char **__restrict, int);
unsigned long long strtoull_l(const char *__restrict, char **__restrict, int,
                              __locale_t);
int system(const char *);
size_t wcstombs(char *__restrict, const wchar_t *__restrict, size_t);
size_t wcstombs_l(char *__restrict, const wchar_t *__restrict, size_t,
                  __locale_t);
__END_DECLS

#define alloca(size) __builtin_alloca(size)

#define atof(str) strtod(str, NULL)
#define atoi(str) ((int)strtol(str, NULL, 10))
#define atol(str) strtol(str, NULL, 10)
#define atoll(str) strtoll(str, NULL, 10)

#define abs(i) __abs(i)
#define labs(i) __labs(i)
#define llabs(i) __llabs(i)

#define div(numer, denom) __div(numer, denom)
#define ldiv(numer, denom) __ldiv(numer, denom)
#define lldiv(numer, denom) __lldiv(numer, denom)

#define bsearch(key, base, nel, width, compar) \
  __bsearch(key, base, nel, width, compar)
#define qsort(base, nel, width, compar) __qsort(base, nel, width, compar)

#endif
