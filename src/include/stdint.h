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

// <stdint.h> - integer types
//
// Features missing:
// - SIG_ATOMIC_MIN and SIG_ATOMIC_MAX:
//   Only needed for signal handling, which is not supported.

#ifndef _STDINT_H_
#define _STDINT_H_

#include <_/limits.h>
#include <_/types.h>

typedef __int8_t int8_t;
typedef __int16_t int16_t;
#ifdef __INT24_TYPE__
typedef __int24_t int24_t;
#endif
typedef __int32_t int32_t;
#ifdef __INT40_TYPE__
typedef __int40_t int40_t;
#endif
#ifdef __INT48_TYPE__
typedef __int48_t int48_t;
#endif
#ifdef __INT56_TYPE__
typedef __int56_t int56_t;
#endif
#ifndef _INT64_T_DECLARED
typedef __int64_t int64_t;
#define _INT64_T_DECLARED
#endif

#ifndef _UINT8_T_DECLARED
typedef __uint8_t uint8_t;
#define _UINT8_T_DECLARED
#endif
#ifndef _UINT16_T_DECLARED
typedef __uint16_t uint16_t;
#define _UINT16_T_DECLARED
#endif
#ifdef __UINT24_TYPE__
typedef __uint24_t uint24_t;
#endif
#ifndef _UINT32_T_DECLARED
typedef __uint32_t uint32_t;
#define _UINT32_T_DECLARED
#endif
#ifdef __UINT40_TYPE__
typedef __uint40_t uint40_t;
#endif
#ifdef __UINT48_TYPE__
typedef __uint48_t uint48_t;
#endif
#ifdef __UINT56_TYPE__
typedef __uint56_t uint56_t;
#endif
#ifndef _UINT64_T_DECLARED
typedef __uint64_t uint64_t;
#define _UINT64_T_DECLARED
#endif

typedef __int_least8_t int_least8_t;
typedef __int_least16_t int_least16_t;
typedef __int_least24_t int_least24_t;
typedef __int_least32_t int_least32_t;
typedef __int_least40_t int_least40_t;
typedef __int_least48_t int_least48_t;
typedef __int_least56_t int_least56_t;
typedef __int_least64_t int_least64_t;

typedef __uint_least8_t uint_least8_t;
typedef __uint_least16_t uint_least16_t;
typedef __uint_least24_t uint_least24_t;
typedef __uint_least32_t uint_least32_t;
typedef __uint_least40_t uint_least40_t;
typedef __uint_least48_t uint_least48_t;
typedef __uint_least56_t uint_least56_t;
typedef __uint_least64_t uint_least64_t;

typedef __int_fast8_t int_fast8_t;
typedef __int_fast16_t int_fast16_t;
typedef __int_fast24_t int_fast24_t;
typedef __int_fast32_t int_fast32_t;
typedef __int_fast40_t int_fast40_t;
typedef __int_fast48_t int_fast48_t;
typedef __int_fast56_t int_fast56_t;
typedef __int_fast64_t int_fast64_t;

typedef __uint_fast8_t uint_fast8_t;
typedef __uint_fast16_t uint_fast16_t;
typedef __uint_fast24_t uint_fast24_t;
typedef __uint_fast32_t uint_fast32_t;
typedef __uint_fast40_t uint_fast40_t;
typedef __uint_fast48_t uint_fast48_t;
typedef __uint_fast56_t uint_fast56_t;
typedef __uint_fast64_t uint_fast64_t;

#ifndef _INTPTR_T_DECLARED
typedef __intptr_t intptr_t;
#define _INTPTR_T_DECLARED
#endif
typedef __uintptr_t uintptr_t;

typedef __intmax_t intmax_t;
typedef __uintmax_t uintmax_t;

#define INT8_MIN _INT8_MIN
#define INT8_MAX _INT8_MAX
#define INT16_MIN _INT16_MIN
#define INT16_MAX _INT16_MAX
#ifdef __INT24_TYPE__
#define INT24_MIN _INT24_MIN
#define INT24_MAX _INT24_MAX
#endif
#define INT32_MIN _INT32_MIN
#define INT32_MAX _INT32_MAX
#ifdef __INT40_TYPE__
#define INT40_MIN _INT40_MIN
#define INT40_MAX _INT40_MAX
#endif
#ifdef __INT48_TYPE__
#define INT48_MIN _INT48_MIN
#define INT48_MAX _INT48_MAX
#endif
#ifdef __INT56_TYPE__
#define INT56_MIN _INT56_MIN
#define INT56_MAX _INT56_MAX
#endif
#define INT64_MIN _INT64_MIN
#define INT64_MAX _INT64_MAX

#define UINT8_MAX _UINT8_MAX
#define UINT16_MAX _UINT16_MAX
#ifdef __UINT24_TYPE__
#define UINT24_MAX _UINT24_MAX
#endif
#define UINT32_MAX _UINT32_MAX
#ifdef __UINT40_TYPE__
#define UINT40_MAX _UINT40_MAX
#endif
#ifdef __UINT48_TYPE__
#define UINT48_MAX _UINT48_MAX
#endif
#ifdef __UINT56_TYPE__
#define UINT56_MAX _UINT56_MAX
#endif
#define UINT64_MAX _UINT64_MAX

#define INT_LEAST8_MIN _INT_LEAST8_MIN
#define INT_LEAST8_MAX _INT_LEAST8_MAX
#define INT_LEAST16_MIN _INT_LEAST16_MIN
#define INT_LEAST16_MAX _INT_LEAST16_MAX
#define INT_LEAST24_MIN _INT_LEAST24_MIN
#define INT_LEAST24_MAX _INT_LEAST24_MAX
#define INT_LEAST32_MIN _INT_LEAST32_MIN
#define INT_LEAST32_MAX _INT_LEAST32_MAX
#define INT_LEAST40_MIN _INT_LEAST40_MIN
#define INT_LEAST40_MAX _INT_LEAST40_MAX
#define INT_LEAST48_MIN _INT_LEAST48_MIN
#define INT_LEAST48_MAX _INT_LEAST48_MAX
#define INT_LEAST56_MIN _INT_LEAST56_MIN
#define INT_LEAST56_MAX _INT_LEAST56_MAX
#define INT_LEAST64_MIN _INT_LEAST64_MIN
#define INT_LEAST64_MAX _INT_LEAST64_MAX

#define UINT_LEAST8_MAX _UINT_LEAST8_MAX
#define UINT_LEAST16_MAX _UINT_LEAST16_MAX
#define UINT_LEAST24_MAX _UINT_LEAST24_MAX
#define UINT_LEAST32_MAX _UINT_LEAST32_MAX
#define UINT_LEAST40_MAX _UINT_LEAST40_MAX
#define UINT_LEAST48_MAX _UINT_LEAST48_MAX
#define UINT_LEAST56_MAX _UINT_LEAST56_MAX
#define UINT_LEAST64_MAX _UINT_LEAST64_MAX

#define INT_FAST8_MIN _INT_FAST8_MIN
#define INT_FAST8_MAX _INT_FAST8_MAX
#define INT_FAST16_MIN _INT_FAST16_MIN
#define INT_FAST16_MAX _INT_FAST16_MAX
#define INT_FAST24_MIN _INT_FAST24_MIN
#define INT_FAST24_MAX _INT_FAST24_MAX
#define INT_FAST32_MIN _INT_FAST32_MIN
#define INT_FAST32_MAX _INT_FAST32_MAX
#define INT_FAST40_MIN _INT_FAST40_MIN
#define INT_FAST40_MAX _INT_FAST40_MAX
#define INT_FAST48_MIN _INT_FAST48_MIN
#define INT_FAST48_MAX _INT_FAST48_MAX
#define INT_FAST56_MIN _INT_FAST56_MIN
#define INT_FAST56_MAX _INT_FAST56_MAX
#define INT_FAST64_MIN _INT_FAST64_MIN
#define INT_FAST64_MAX _INT_FAST64_MAX

#define UINT_FAST8_MAX _UINT_FAST8_MAX
#define UINT_FAST16_MAX _UINT_FAST16_MAX
#define UINT_FAST24_MAX _UINT_FAST24_MAX
#define UINT_FAST32_MAX _UINT_FAST32_MAX
#define UINT_FAST40_MAX _UINT_FAST40_MAX
#define UINT_FAST48_MAX _UINT_FAST48_MAX
#define UINT_FAST56_MAX _UINT_FAST56_MAX
#define UINT_FAST64_MAX _UINT_FAST64_MAX

#define INTPTR_MIN _INTPTR_MIN
#define INTPTR_MAX _INTPTR_MAX
#define UINTPTR_MAX _UINTPTR_MAX

#define INTMAX_MIN _INTMAX_MIN
#define INTMAX_MAX _INTMAX_MAX
#define UINTMAX_MAX _UINTMAX_MAX

#define PTRDIFF_MIN _PTRDIFF_MIN
#define PTRDIFF_MAX _PTRDIFF_MAX

#define SIZE_MAX _SIZE_MAX

#define WCHAR_MIN _WCHAR_MIN
#define WCHAR_MAX _WCHAR_MAX

#define WINT_MIN _WINT_MIN
#define WINT_MAX _WINT_MAX

#define INT8_C(value) _INT8_C(value)
#define INT16_C(value) _INT16_C(value)
#define INT24_C(value) _INT24_C(value)
#define INT32_C(value) _INT32_C(value)
#define INT40_C(value) _INT40_C(value)
#define INT48_C(value) _INT48_C(value)
#define INT56_C(value) _INT56_C(value)
#define INT64_C(value) _INT64_C(value)

#define UINT8_C(value) _UINT8_C(value)
#define UINT16_C(value) _UINT16_C(value)
#define UINT24_C(value) _UINT24_C(value)
#define UINT32_C(value) _UINT32_C(value)
#define UINT40_C(value) _UINT40_C(value)
#define UINT48_C(value) _UINT48_C(value)
#define UINT56_C(value) _UINT56_C(value)
#define UINT64_C(value) _UINT64_C(value)

#define INTMAX_C(value) _INTMAX_C(value)
#define UINTMAX_C(value) _UINTMAX_C(value)

#endif
