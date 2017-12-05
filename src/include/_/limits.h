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

#ifndef ___LIMITS_H_
#define ___LIMITS_H_

#define _INTN_C(c, s) __INTN_C(c, s)
#define __INTN_C(c, s) (c##s)

// Standard signed integer types.

#define _INT8_MIN (-_INT8_MAX - 1)
#define _INT8_MAX _INT8_C(0x7f)
#define _INT16_MIN (-_INT16_MAX - 1)
#define _INT16_MAX _INT16_C(0x7fff)
#ifdef __INT24_TYPE__
#define _INT24_MIN (-_INT24_MAX - 1)
#define _INT24_MAX _INT24_C(0x7fffff)
#endif
#define _INT32_MIN (-_INT32_MAX - 1)
#define _INT32_MAX _INT32_C(0x7fffffff)
#ifdef __INT40_TYPE__
#define _INT40_MIN (-_INT40_MAX - 1)
#define _INT40_MAX _INT40_C(0x7fffffffff)
#endif
#ifdef __INT48_TYPE__
#define _INT48_MIN (-_INT48_MAX - 1)
#define _INT48_MAX _INT48_C(0x7fffffffffff)
#endif
#ifdef __INT56_TYPE__
#define _INT56_MIN (-_INT56_MAX - 1)
#define _INT56_MAX _INT56_C(0x7fffffffffffff)
#endif
#define _INT64_MIN (-_INT64_MAX - 1)
#define _INT64_MAX _INT64_C(0x7fffffffffffffff)

#define _INT_LEAST8_MIN _INT8_MIN
#define _INT_LEAST8_MAX _INT8_MAX
#define _INT8_C(c) _INTN_C(c, __INT8_C_SUFFIX__)
#define _INT_LEAST16_MIN _INT16_MIN
#define _INT_LEAST16_MAX _INT16_MAX
#define _INT16_C(c) _INTN_C(c, __INT16_C_SUFFIX__)
#ifdef __INT24_TYPE__
#define _INT_LEAST24_MIN _INT24_MIN
#define _INT_LEAST24_MAX _INT24_MAX
#define _INT24_C(c) _INTN_C(c, __INT24_C_SUFFIX__)
#else
#define _INT_LEAST24_MIN _INT_LEAST32_MIN
#define _INT_LEAST24_MAX _INT_LEAST32_MAX
#define _INT24_C(c) _INT32_C(c)
#endif
#define _INT_LEAST32_MAX _INT32_MAX
#define _INT_LEAST32_MIN _INT32_MIN
#define _INT32_C(c) _INTN_C(c, __INT32_C_SUFFIX__)
#ifdef __INT40_TYPE__
#define _INT_LEAST40_MIN _INT40_MIN
#define _INT_LEAST40_MAX _INT40_MAX
#define _INT40_C(c) _INTN_C(c, __INT40_C_SUFFIX__)
#else
#define _INT_LEAST40_MIN _INT_LEAST48_MIN
#define _INT_LEAST40_MAX _INT_LEAST48_MAX
#define _INT40_C(c) _INT48_C(c)
#endif
#ifdef __INT48_TYPE__
#define _INT_LEAST48_MIN _INT48_MIN
#define _INT_LEAST48_MAX _INT48_MAX
#define _INT48_C(c) _INTN_C(c, __INT48_C_SUFFIX__)
#else
#define _INT_LEAST48_MIN _INT_LEAST56_MIN
#define _INT_LEAST48_MAX _INT_LEAST56_MAX
#define _INT48_C(c) _INT56_C(c)
#endif
#ifdef __INT56_TYPE__
#define _INT_LEAST56_MIN _INT56_MIN
#define _INT_LEAST56_MAX _INT56_MAX
#define _INT56_C(c) _INTN_C(c, __INT56_C_SUFFIX__)
#else
#define _INT_LEAST56_MIN _INT_LEAST64_MIN
#define _INT_LEAST56_MAX _INT_LEAST64_MAX
#define _INT56_C(c) _INT64_C(c)
#endif
#define _INT_LEAST64_MIN _INT64_MIN
#define _INT_LEAST64_MAX _INT64_MAX
#define _INT64_C(c) _INTN_C(c, __INT64_C_SUFFIX__)

#define _INT_FAST8_MIN _INT_LEAST8_MIN
#define _INT_FAST8_MAX _INT_LEAST8_MAX
#define _INT_FAST16_MIN _INT_LEAST16_MIN
#define _INT_FAST16_MAX _INT_LEAST16_MAX
#define _INT_FAST24_MIN _INT_LEAST24_MIN
#define _INT_FAST24_MAX _INT_LEAST24_MAX
#define _INT_FAST32_MIN _INT_LEAST32_MIN
#define _INT_FAST32_MAX _INT_LEAST32_MAX
#define _INT_FAST40_MIN _INT_LEAST40_MIN
#define _INT_FAST40_MAX _INT_LEAST40_MAX
#define _INT_FAST48_MIN _INT_LEAST48_MIN
#define _INT_FAST48_MAX _INT_LEAST48_MAX
#define _INT_FAST56_MIN _INT_LEAST56_MIN
#define _INT_FAST56_MAX _INT_LEAST56_MAX
#define _INT_FAST64_MIN _INT_LEAST64_MIN
#define _INT_FAST64_MAX _INT_LEAST64_MAX

// Standard unsigned integer types.

#define _UINT8_MAX _UINT8_C(0xff)
#define _UINT16_MAX _UINT16_C(0xffff)
#ifdef __UINT24_TYPE__
#define _UINT24_MAX _UINT24_C(0xffffff)
#endif
#define _UINT32_MAX _UINT32_C(0xffffffff)
#ifdef __UINT40_TYPE__
#define _UINT40_MAX _UINT40_C(0xffffffffff)
#endif
#ifdef __UINT48_TYPE__
#define _UINT48_MAX _UINT48_C(0xffffffffffff)
#endif
#ifdef __UINT56_TYPE__
#define _UINT56_MAX _UINT56_C(0xffffffffffffff)
#endif
#define _UINT64_MAX _UINT64_C(0xffffffffffffffff)

#define _UINT_LEAST8_MAX _UINT8_MAX
#define _UINT8_C(c) _INTN_C(c, __UINT8_C_SUFFIX__)
#define _UINT_LEAST16_MAX _UINT16_MAX
#define _UINT16_C(c) _INTN_C(c, __UINT16_C_SUFFIX__)
#ifdef __UINT24_TYPE__
#define _UINT_LEAST24_MAX _UINT24_MAX
#define _UINT24_C(c) _INTN_C(c, __UINT24_C_SUFFIX__)
#else
#define _UINT_LEAST24_MAX _UINT_LEAST32_MAX
#define _UINT24_C(c) _UINT32_C(c)
#endif
#define _UINT_LEAST32_MAX _UINT32_MAX
#define _UINT32_C(c) _INTN_C(c, __UINT32_C_SUFFIX__)
#ifdef __UINT40_TYPE__
#define _UINT_LEAST40_MAX _UINT40_MAX
#define _UINT40_C(c) _INTN_C(c, __UINT40_C_SUFFIX__)
#else
#define _UINT_LEAST40_MAX _UINT_LEAST48_MAX
#define _UINT40_C(c) _UINT48_C(c)
#endif
#ifdef __UINT48_TYPE__
#define _UINT_LEAST48_MAX _UINT48_MAX
#define _UINT48_C(c) _INTN_C(c, __UINT48_C_SUFFIX__)
#else
#define _UINT_LEAST48_MAX _UINT_LEAST56_MAX
#define _UINT48_C(c) _UINT56_C(c)
#endif
#ifdef __UINT56_TYPE__
#define _UINT_LEAST56_MAX _UINT56_MAX
#define _UINT56_C(c) _INTN_C(c, __UINT56_C_SUFFIX__)
#else
#define _UINT_LEAST56_MAX _UINT_LEAST64_MAX
#define _UINT56_C(c) _UINT64_C(c)
#endif
#define _UINT_LEAST64_MAX _UINT64_MAX
#define _UINT64_C(c) _INTN_C(c, __UINT64_C_SUFFIX__)

#define _UINT_FAST8_MAX _UINT_LEAST8_MAX
#define _UINT_FAST16_MAX _UINT_LEAST16_MAX
#define _UINT_FAST24_MAX _UINT_LEAST24_MAX
#define _UINT_FAST32_MAX _UINT_LEAST32_MAX
#define _UINT_FAST40_MAX _UINT_LEAST40_MAX
#define _UINT_FAST48_MAX _UINT_LEAST48_MAX
#define _UINT_FAST56_MAX _UINT_LEAST56_MAX
#define _UINT_FAST64_MAX _UINT_LEAST64_MAX

// Language native types.
// TODO(ed): Isn't there a way to derive _*_BIT more easily?

#define _CHAR_BIT __CHAR_BIT__
#if __SIZEOF_SHORT__ * _CHAR_BIT == 16
#define _SHRT_BIT 16
#else
#error "Size of short unknown"
#endif
#if __SIZEOF_INT__ * _CHAR_BIT == 32
#define _INT_BIT 32
#else
#error "Size of int unknown"
#endif
#if __SIZEOF_LONG__ * _CHAR_BIT == 32
#define _LONG_BIT 32
#elif __SIZEOF_LONG__ * _CHAR_BIT == 64
#define _LONG_BIT 64
#else
#error "Size of long unknown"
#endif
#if __SIZEOF_LONG_LONG__ * _CHAR_BIT == 64
#define _LLONG_BIT 64
#else
#error "Size of long long unknown"
#endif

#define _SCHAR_MIN (-_SCHAR_MAX - 1)
#define _SCHAR_MAX __SCHAR_MAX__
#define _SHRT_MIN (-_SHRT_MAX - 1)
#define _SHRT_MAX __SHRT_MAX__
#define _INT_MIN (-_INT_MAX - 1)
#define _INT_MAX __INT_MAX__
#define _LONG_MIN (-_LONG_MAX - 1)
#define _LONG_MAX __LONG_MAX__
#define _LLONG_MIN (-_LLONG_MAX - 1)
#define _LLONG_MAX __LONG_LONG_MAX__

#define _UCHAR_MAX (_SCHAR_MAX * 2 + 1)
#define _USHRT_MAX (_SHRT_MAX * 2 + 1)
#define _UINT_MAX (_INT_MAX * 2U + 1U)
#define _ULONG_MAX (_LONG_MAX * 2UL + 1UL)
#define _ULLONG_MAX (_LLONG_MAX * 2ULL + 1ULL)

#ifdef __CHAR_UNSIGNED__
#define _CHAR_MIN 0
#define _CHAR_MAX _UCHAR_MAX
#else
#define _CHAR_MIN _SCHAR_MIN
#define _CHAR_MAX _SCHAR_MAX
#endif

// Other machine-dependent type definitions.

#define _INTPTR_MIN (-_INTPTR_MAX - 1)
#define _INTPTR_MAX __INTPTR_MAX__
#define _UINTPTR_MAX __UINTPTR_MAX__

#define _INTMAX_MIN (-_INTMAX_MAX - 1)
#define _INTMAX_MAX __INTMAX_MAX__
#define _INTMAX_C(c) _INTN_C(c, __INTMAX_C_SUFFIX__)
#define _UINTMAX_MAX __UINTMAX_MAX__
#define _UINTMAX_C(c) _INTN_C(c, __UINTMAX_C_SUFFIX__)

#define _PTRDIFF_MIN (-_PTRDIFF_MAX - 1)
#define _PTRDIFF_MAX __PTRDIFF_MAX__

#define _SSIZE_MAX _PTRDIFF_MAX
#define _SIZE_MAX __SIZE_MAX__

// Compatibility for versions of Clang that don't define __WINT_MAX__.
// TODO(ed): Remove this once Clang 6.0 has been released.
#ifndef __WINT_MAX__
_Static_assert(__WINT_WIDTH__ == 32, "Unsupported wint_t size");
#ifdef __WINT_UNSIGNED__
#define __WINT_MAX__ _UINT32_MAX
#else
#define __WINT_MAX__ _INT32_MAX
#endif
#endif

#ifdef __WCHAR_UNSIGNED__
#define _WCHAR_MIN 0
#else
#define _WCHAR_MIN (-_WCHAR_MAX - 1)
#endif
#define _WCHAR_MAX __WCHAR_MAX__
#ifdef __WINT_UNSIGNED__
#define _WINT_MIN 0
#else
#define _WINT_MIN (-_WINT_MAX - 1)
#endif
#define _WINT_MAX __WINT_MAX__

#endif
