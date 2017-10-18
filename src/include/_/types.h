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

#ifndef ___TYPES_H_
#define ___TYPES_H_

#include <_/cdefs.h>

#ifdef __cplusplus
#if __cplusplus >= 201103L
#define _NULL nullptr
#else
#define _NULL 0
#endif
#else
#define _NULL ((void *)0)
#endif

// Standard signed integer types.

typedef __INT8_TYPE__ __int8_t;
typedef __INT16_TYPE__ __int16_t;
#ifdef __INT24_TYPE__
typedef __INT24_TYPE__ __int24_t;
#endif
typedef __INT32_TYPE__ __int32_t;
#ifdef __INT40_TYPE__
typedef __INT40_TYPE__ __int40_t;
#endif
#ifdef __INT48_TYPE__
typedef __INT48_TYPE__ __int48_t;
#endif
#ifdef __INT56_TYPE__
typedef __INT56_TYPE__ __int56_t;
#endif
typedef __INT64_TYPE__ __int64_t;

typedef __int64_t __int_least64_t;
#ifdef __INT56_TYPE__
typedef __int56_t __int_least56_t;
#else
typedef __int_least64_t __int_least56_t;
#endif
#ifdef __INT48_TYPE__
typedef __int48_t __int_least48_t;
#else
typedef __int_least56_t __int_least48_t;
#endif
#ifdef __INT40_TYPE__
typedef __INT40_TYPE__ __int40_t;
typedef __int40_t __int_least40_t;
#else
typedef __int_least48_t __int_least40_t;
#endif
typedef __int32_t __int_least32_t;
#ifdef __INT24_TYPE__
typedef __int24_t __int_least24_t;
#else
typedef __int_least32_t __int_least24_t;
#endif
typedef __int16_t __int_least16_t;
typedef __int8_t __int_least8_t;

typedef __int_least8_t __int_fast8_t;
typedef __int_least16_t __int_fast16_t;
typedef __int_least24_t __int_fast24_t;
typedef __int_least32_t __int_fast32_t;
typedef __int_least40_t __int_fast40_t;
typedef __int_least48_t __int_fast48_t;
typedef __int_least56_t __int_fast56_t;
typedef __int_least64_t __int_fast64_t;

// Unsigned integer types.

typedef __UINT8_TYPE__ __uint8_t;
typedef __UINT16_TYPE__ __uint16_t;
#ifdef __UINT24_TYPE__
typedef __UINT24_TYPE__ __uint24_t;
#endif
typedef __UINT32_TYPE__ __uint32_t;
#ifdef __UINT40_TYPE__
typedef __UINT40_TYPE__ __uint40_t;
#endif
#ifdef __UINT48_TYPE__
typedef __UINT48_TYPE__ __uint48_t;
#endif
#ifdef __UINT56_TYPE__
typedef __UINT56_TYPE__ __uint56_t;
#endif
typedef __UINT64_TYPE__ __uint64_t;

typedef __uint64_t __uint_least64_t;
#ifdef __UINT56_TYPE__
typedef __uint56_t __uint_least56_t;
#else
typedef __uint_least64_t __uint_least56_t;
#endif
#ifdef __UINT48_TYPE__
typedef __uint48_t __uint_least48_t;
#else
typedef __uint_least56_t __uint_least48_t;
#endif
#ifdef __UINT40_TYPE__
typedef __uint40_t __uint_least40_t;
#else
typedef __uint_least48_t __uint_least40_t;
#endif
typedef __uint32_t __uint_least32_t;
#ifdef __UINT24_TYPE__
typedef __uint24_t __uint_least24_t;
#else
typedef __uint_least32_t __uint_least24_t;
#endif
typedef __uint16_t __uint_least16_t;
typedef __uint8_t __uint_least8_t;

typedef __uint_least8_t __uint_fast8_t;
typedef __uint_least16_t __uint_fast16_t;
typedef __uint_least24_t __uint_fast24_t;
typedef __uint_least32_t __uint_fast32_t;
typedef __uint_least40_t __uint_fast40_t;
typedef __uint_least48_t __uint_fast48_t;
typedef __uint_least56_t __uint_fast56_t;
typedef __uint_least64_t __uint_fast64_t;

// Other machine-dependent type definitions.

#define ___intn_t(width) __int##width##_t
#define __intn_t(width) ___intn_t(width)
#define ___uintn_t(width) __uint##width##_t
#define __uintn_t(width) ___uintn_t(width)

typedef __intn_t(__INTPTR_WIDTH__) __intptr_t;
typedef __uintn_t(__INTPTR_WIDTH__) __uintptr_t;

typedef __intn_t(__INTMAX_WIDTH__) __intmax_t;
typedef __uintn_t(__INTMAX_WIDTH__) __uintmax_t;

typedef __intn_t(__PTRDIFF_WIDTH__) __ptrdiff_t;

typedef __intn_t(__SIZE_WIDTH__) __ssize_t;
typedef __uintn_t(__SIZE_WIDTH__) __size_t;

#ifdef __WCHAR_UNSIGNED__
typedef __uintn_t(__WCHAR_WIDTH__) __wchar_t;
#else
typedef __intn_t(__WCHAR_WIDTH__) __wchar_t;
#endif

typedef __intn_t(__WINT_WIDTH__) __wint_t;

#undef ___intn_t
#undef __intn_t
#undef ___uintn_t
#undef __uintn_t

typedef struct {
  _Alignas(_Alignof(__intmax_t)) __intmax_t __a;
  _Alignas(_Alignof(long double)) long double __b;
} __max_align_t;

// Machine-independent definitions.

#ifdef __cplusplus
typedef bool _Bool;
#endif
#if !defined(__clang__) || !defined(__cplusplus)
typedef __uint_least16_t __char16_t;
typedef __uint_least32_t __char32_t;
#endif
typedef __uint64_t __clock_t;
typedef const struct __clockid *__clockid_t;
typedef __uint64_t __dev_t;
typedef __uint8_t __filetype_t;
typedef __uint32_t __in_addr_t;
typedef __uint16_t __in_port_t;
typedef __uint64_t __ino_t;
typedef struct __locale *__locale_t;
typedef struct {
  __char16_t __surrogate;    // UTF-16: Leading or trailing surrogate.
  unsigned int __bytesleft;  // UTF-8/GB 2312: Number of bytes until completion.
  __char32_t __partial;      // UTF-8/GB 2312: Partially completed character.
  __char32_t __lowerbound;   // UTF-8: Lowerbound on character.
} __mbstate_t;
typedef __uint16_t __mode_t;
typedef __uint32_t __nlink_t;
typedef __int64_t __off_t;
typedef struct {
  struct __pthread_key *__key;
} __pthread_key_t;
typedef struct __pthread *__pthread_t;
typedef __uint8_t __sa_family_t;
typedef struct __siginfo __siginfo_t;
typedef struct {
  __uint32_t __mask;
} __sigset_t;
typedef __int_least24_t __suseconds_t;
typedef __int64_t __time_t;
typedef __uint64_t __useconds_t;
typedef __builtin_va_list __va_list;

// Process-shared threading objects with a fixed ABI.

typedef struct {
  _Atomic(__uint32_t) __waiters;
  __uint32_t __clock;
  __uint8_t __pshared;
} __pthread_cond_t;
typedef struct __lockable __pthread_lock {
  _Atomic(__uint32_t) __state;  // Kernelspace futex.
  __int32_t __write_recursion;  // Userspace write recursion counter.
  __uint8_t __pshared;
  struct {
    _Alignas(__uint64_t) struct __pthread_lock *__l_next;
    _Alignas(__uint64_t) struct __pthread_lock **__l_prev;
  } __write_locks;  // List pointers for per-thread list of locks.
} __pthread_lock_t;
typedef struct {
  __pthread_lock_t __lock;
  __pthread_cond_t __cond;
  __uint32_t __init;
  __uint32_t __remaining;
  __uint32_t __generation;
} __pthread_barrier_t;
typedef struct {
  _Atomic(__uint32_t) __state;
} __pthread_once_t;
typedef struct {
  __pthread_lock_t __lock;
  __pthread_cond_t __cond;
  __uint32_t __value;
} __sem_t;

_Static_assert(sizeof(__pthread_barrier_t) == 56, "ABI broken");
_Static_assert(sizeof(__pthread_cond_t) == 12, "ABI broken");
_Static_assert(sizeof(__pthread_lock_t) == 32, "ABI broken");
_Static_assert(sizeof(__pthread_once_t) == 4, "ABI broken");
_Static_assert(sizeof(__sem_t) == 48, "ABI broken");

typedef struct {
  int __detachstate;
  __size_t __stacksize;
} __pthread_attr_t;
typedef struct {
  int __pshared;
} __pthread_barrierattr_t;
typedef struct {
  __clockid_t __clock;
  int __pshared;
} __pthread_condattr_t;
typedef struct {
  int __pshared;
  int __type;
} __pthread_lockattr_t;

// wchar_t, char16_t and char32_t are separate language keywords in C++.

#ifdef __cplusplus
#define _WCHAR_T_DECLARED
#if __cplusplus >= 201103L
#define _CHAR16_T_DECLARED
#define _CHAR32_T_DECLARED
#endif
#endif

#endif
