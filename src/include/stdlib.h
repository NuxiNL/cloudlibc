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

// <stdlib.h> - standard library definitions
//
// Extensions:
// - MB_CUR_MAX_L(), mblen_l(), mbstowcs_l(), mbtowc_l(), wcstombs_l()
//   and wctomb_l():
//   Regular functions always use the C locale. Available on many other
//   operating systems.
// - alloca():
//   Present on most other operating systems.
// - arc4random(), arc4random_buf() and arc4random_uniform():
//   Secure random number generator. Available on many other operating
//   systems.
// - l64a_r():
//   Thread-safe replacement for l64a(). Part of the SVID, 4th edition.
// - qsort_r():
//   Available on many other operating systems, although the prototype
//   is not consistent. This implementation is compatible with glibc.
//   It is expected that this version will be standardized in the future.
// - reallocarray():
//   Allows for reallocation of buffers without integer overflows.
//
// Features missing:
// - initstate(), lcong48(), seed48(), setstate(), srand(), srand48()
//   and srandom():
//   Randomizer is seeded securely by default. There is no need to seed
//   manually.
// - WEXITSTATUS(), WIFEXITED(), WIFSIGNALED(), WIFSTOPPED(), WNOHANG,
//   WSTOPSIG(), WTERMSIG(), WUNTRACED:
//   Only useful if system() would actually work.
// - l64a():
//   Not thread-safe. Use l64a_r() instead.
// - putenv(), setenv() and unsetenv():
//   Environment variables are not available.
// - grantpt(), posix_openpt(), ptsname() and unlockpt():
//   Pseudo-terminals are not available.
// - mkdtemp(), mkstemp() and realpath():
//   Requires global filesystem namespace.
// - setkey():
//   Password database and encryption schemes not available.
// - system():
//   Requires a command shell.

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
#define MB_CUR_MAX ((size_t)1)

// Keep existing code happy that assumes that MB_CUR_MAX_L is a macro.
#define MB_CUR_MAX_L MB_CUR_MAX_L

#define alloca(size) __builtin_alloca(size)

__BEGIN_DECLS
_Noreturn void _Exit(int);
size_t MB_CUR_MAX_L(__locale_t);
long a64l(const char *);
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
double erand48(__uint16_t *);
_Noreturn void exit(int);
void free(void *);
char *getenv(const char *);
int getsubopt(char **, char *const *, char **);
long jrand48(__uint16_t *);
int l64a_r(long, char *, int);
long labs(long) __pure2;
ldiv_t ldiv(long, long) __pure2;
long long llabs(long long) __pure2;
lldiv_t lldiv(long long, long long) __pure2;
long lrand48(void);
void *malloc(size_t) __malloc_like;
int mblen(const char *, size_t);
int mblen_l(const char *, size_t, __locale_t);
size_t mbstowcs(wchar_t *__restrict, const char *__restrict, size_t);
size_t mbstowcs_l(wchar_t *__restrict, const char *__restrict, size_t,
                  __locale_t);
int mbtowc(wchar_t *__restrict, const char *__restrict, size_t);
int mbtowc_l(wchar_t *__restrict, const char *__restrict, size_t, __locale_t);
long mrand48(void);
long nrand48(__uint16_t *);
int posix_memalign(void **, size_t, size_t);
void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
void qsort_r(void *, size_t, size_t,
             int (*)(const void *, const void *, void *), void *);
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
size_t wcstombs(char *__restrict, const wchar_t *__restrict, size_t);
size_t wcstombs_l(char *__restrict, const wchar_t *__restrict, size_t,
                  __locale_t);
int wctomb(char *, wchar_t);
int wctomb_l(char *, wchar_t, __locale_t);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline double __atof(const char *__str) {
  return strtod(__str, NULL);
}
#define atof(str) __atof(str)

static __inline int __atoi(const char *__str) {
  return (int)strtol(__str, NULL, 10);
}
#define atoi(str) __atoi(str)

static __inline long __atol(const char *__str) {
  return strtol(__str, NULL, 10);
}
#define atol(str) __atol(str)

static __inline long long __atoll(const char *__str) {
  return strtoll(__str, NULL, 10);
}
#define atoll(str) __atoll(str)

static __inline int __abs(int __i) {
  return __i < 0 ? -__i : __i;
}
#define abs(i) __abs(i)

static __inline long __labs(long __i) {
  return __i < 0 ? -__i : __i;
}
#define labs(i) __labs(i)

static __inline long long __llabs(long long __i) {
  return __i < 0 ? -__i : __i;
}
#define llabs(i) __llabs(i)

static __inline div_t __div(int __numer, int __denom) {
  div_t __res = {__numer / __denom, __numer % __denom};
  return __res;
}
#define div(numer, denom) __div(numer, denom)

static __inline ldiv_t __ldiv(long __numer, long __denom) {
  ldiv_t __res = {__numer / __denom, __numer % __denom};
  return __res;
}
#define ldiv(numer, denom) __ldiv(numer, denom)

static __inline lldiv_t __lldiv(long long __numer, long long __denom) {
  lldiv_t __res = {__numer / __denom, __numer % __denom};
  return __res;
}
#define lldiv(numer, denom) __lldiv(numer, denom)

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
#define bsearch(key, base, nel, width, compar) \
  __preserve_const(void, __bsearch, base, key, base, nel, width, compar)

// qsort_r() implementation from Bentley and McIlroy's
// "Engineering a Sort Function".
//
// This sorting function is inlined into this header, so that the
// compiler can create an optimized version that takes the alignment and
// size of the elements into account. It also reduces the overhead of
// indirect function calls.

static __inline void __qsort_r(void *, size_t, size_t,
                               int (*)(const void *, const void *, void *),
                               void *);

static __inline size_t __qsort_min(size_t __a, size_t __b) {
  return __a < __b ? __a : __b;
}

// Swaps the contents of two buffers.
static __inline void __qsort_swap(char *__a, char *__b, size_t __n) {
  char __t;

  while (__n-- > 0) {
    __t = *__a;
    *__a++ = *__b;
    *__b++ = __t;
  }
}

// Implementation of insertionsort for small lists.
static __inline void __qsort_insertionsort(
    char *__a, size_t __nel, size_t __width,
    int (*__cmp)(const void *, const void *, void *), void *__thunk) {
  char *__pm, *__pl;

  for (__pm = __a + __width; __pm < __a + __nel * __width; __pm += __width)
    for (__pl = __pm; __pl > __a && __cmp(__pl - __width, __pl, __thunk) > 0;
         __pl -= __width)
      __qsort_swap(__pl, __pl - __width, __width);
}

// Returns the median of three elements.
static __inline char *__qsort_med3(char *__a, char *__b, char *__c,
                                   int (*__cmp)(const void *, const void *,
                                                void *),
                                   void *__thunk) {
  return __cmp(__a, __b, __thunk) < 0
             ? (__cmp(__b, __c, __thunk) < 0
                    ? __b
                    : __cmp(__a, __c, __thunk) < 0 ? __c : __a)
             : (__cmp(__b, __c, __thunk) > 0
                    ? __b
                    : __cmp(__a, __c, __thunk) > 0 ? __c : __a);
}

// Picks a pivot based on a pseudo-median of three or nine.
// TODO(ed): Does this still guarantee an O(n log n) running time?
static __inline char *__qsort_pickpivot(char *__a, size_t __nel, size_t __width,
                                        int (*__cmp)(const void *, const void *,
                                                     void *),
                                        void *__thunk) {
  char *__pl, *__pm, *__pn;
  size_t __s;

  __pl = __a;
  __pm = __a + (__nel / 2) * __width;
  __pn = __a + (__nel - 1) * __width;
  if (__nel > 40) {
    __s = (__nel / 8) * __width;
    __pl = __qsort_med3(__pl, __pl + __s, __pl + 2 * __s, __cmp, __thunk);
    __pm = __qsort_med3(__pm - __s, __pm, __pm + __s, __cmp, __thunk);
    __pn = __qsort_med3(__pn - 2 * __s, __pn - __s, __pn, __cmp, __thunk);
  }
  return __qsort_med3(__pl, __pm, __pn, __cmp, __thunk);
}

// Implementation of quicksort for larger lists.
static __inline void __qsort_quicksort(char *__a, size_t __nel, size_t __width,
                                       int (*__cmp)(const void *, const void *,
                                                    void *),
                                       void *__thunk) {
  char *__pa, *__pb, *__pc, *__pd, *__pn;
  int __r;
  size_t __s;

  // Select pivot and move it to the head of the list.
  __qsort_swap(__a, __qsort_pickpivot(__a, __nel, __width, __cmp, __thunk),
               __width);

  // Perform partitioning.
  __pa = __pb = __a;
  __pc = __pd = __a + (__nel - 1) * __width;
  for (;;) {
    while (__pb <= __pc && (__r = __cmp(__pb, __a, __thunk)) <= 0) {
      if (__r == 0) {
        __qsort_swap(__pa, __pb, __width);
        __pa += __width;
      }
      __pb += __width;
    }
    while (__pc >= __pb && (__r = __cmp(__pc, __a, __thunk)) >= 0) {
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

  // Store pivot between the two partitions.
  __pn = __a + __nel * __width;
  __s = __qsort_min((size_t)(__pa - __a), (size_t)(__pb - __pa));
  __qsort_swap(__a, __pb - __s, __s);
  __s = __qsort_min((size_t)(__pd - __pc), (size_t)(__pn - __pd) - __width);
  __qsort_swap(__pb, __pn - __s, __s);

  // Sort the two partitions.
  __s = (size_t)(__pb - __pa);
  __qsort_r(__a, __s / __width, __width, __cmp, __thunk);
  __s = (size_t)(__pd - __pc);
  __qsort_r(__pn - __s, __s / __width, __width, __cmp, __thunk);
}

static __inline void __qsort_r(void *__base, size_t __nel, size_t __width,
                               int (*__cmp)(const void *, const void *, void *),
                               void *__thunk) {
  char *__a;

  __a = (char *)__base;
  if (__nel < 8) {
    __qsort_insertionsort(__a, __nel, __width, __cmp, __thunk);
  } else {
    __qsort_quicksort(__a, __nel, __width, __cmp, __thunk);
  }
}
#define qsort_r(base, nel, width, compar, thunk) \
  __qsort_r(base, nel, width, compar, thunk)

// qsort(): Call into qsort_r(), providing the callback as the thunk.
// We assume that the optimizer is smart enough to simplify.

static __inline int __qsort_cmp(const void *__a, const void *__b,
                                void *__thunk) {
  return ((int (*)(const void *, const void *))__thunk)(__a, __b);
}

static __inline void __qsort(void *__base, size_t __nel, size_t __width,
                             int (*__cmp)(const void *, const void *)) {
  qsort_r(__base, __nel, __width, __qsort_cmp, (void *)__cmp);
}
#define qsort(base, nel, width, compar) __qsort(base, nel, width, compar)
#endif

#endif
