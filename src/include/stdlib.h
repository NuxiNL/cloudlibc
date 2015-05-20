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
//   Not thread-safe. Use <wchar.h> instead.
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

#endif
