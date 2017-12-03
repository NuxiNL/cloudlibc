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

// <stdatomic.h> - atomics
//
// Extensions:
// - __memory_order_hle_acquire and __memory_order_hle_release:
//   Allows for annotation of locks to reduce contention.

#ifndef _STDATOMIC_H_
#define _STDATOMIC_H_

#include <_/types.h>

#define ATOMIC_BOOL_LOCK_FREE __GCC_ATOMIC_BOOL_LOCK_FREE
#define ATOMIC_CHAR_LOCK_FREE __GCC_ATOMIC_CHAR_LOCK_FREE
#define ATOMIC_CHAR16_T_LOCK_FREE __GCC_ATOMIC_CHAR16_T_LOCK_FREE
#define ATOMIC_CHAR32_T_LOCK_FREE __GCC_ATOMIC_CHAR32_T_LOCK_FREE
#define ATOMIC_WCHAR_T_LOCK_FREE __GCC_ATOMIC_WCHAR_T_LOCK_FREE
#define ATOMIC_SHORT_LOCK_FREE __GCC_ATOMIC_SHORT_LOCK_FREE
#define ATOMIC_INT_LOCK_FREE __GCC_ATOMIC_INT_LOCK_FREE
#define ATOMIC_LONG_LOCK_FREE __GCC_ATOMIC_LONG_LOCK_FREE
#define ATOMIC_LLONG_LOCK_FREE __GCC_ATOMIC_LLONG_LOCK_FREE
#define ATOMIC_POINTER_LOCK_FREE __GCC_ATOMIC_POINTER_LOCK_FREE

#define ATOMIC_FLAG_INIT \
  { ATOMIC_VAR_INIT(0) }
#define ATOMIC_VAR_INIT(value) (value)

#ifdef __clang__
#define atomic_init(object, value) __c11_atomic_init(object, value)
#else
#define atomic_init(object, value) \
  do {                             \
    *(object) = (value);           \
  } while (0)
#endif

typedef enum {
  memory_order_relaxed = __ATOMIC_RELAXED,
  memory_order_consume = __ATOMIC_CONSUME,
  memory_order_acquire = __ATOMIC_ACQUIRE,
  memory_order_release = __ATOMIC_RELEASE,
  memory_order_acq_rel = __ATOMIC_ACQ_REL,
  memory_order_seq_cst = __ATOMIC_SEQ_CST,

#if defined(__ATOMIC_HLE_ACQUIRE) && defined(__ATOMIC_HLE_RELEASE)
  // Compiler supports Hardware Lock Elision.
  __memory_order_hle_acquire = __ATOMIC_HLE_ACQUIRE,
  __memory_order_hle_release = __ATOMIC_HLE_RELEASE
#else
  // Ignore Hardware Lock Elision hints.
  __memory_order_hle_acquire = 0,
  __memory_order_hle_release = 0
#endif
} memory_order;

#define kill_dependency(y) (y)

__BEGIN_DECLS
void atomic_thread_fence(memory_order);
void atomic_signal_fence(memory_order);
__END_DECLS

#ifdef __clang__
#define atomic_thread_fence(order) __c11_atomic_thread_fence(order)
#define atomic_signal_fence(order) __c11_atomic_signal_fence(order)
#define atomic_is_lock_free(object) __c11_atomic_is_lock_free(sizeof(*(object)))
#else
#define atomic_thread_fence(order) __atomic_thread_fence(order)
#define atomic_signal_fence(order) __atomic_signal_fence(order)
#define atomic_is_lock_free(object) \
  __atomic_is_lock_free(sizeof(*(object)), object)
#endif

typedef _Atomic(_Bool) atomic_bool;
typedef _Atomic(char) atomic_char;
typedef _Atomic(signed char) atomic_schar;
typedef _Atomic(unsigned char) atomic_uchar;
typedef _Atomic(short) atomic_short;
typedef _Atomic(unsigned short) atomic_ushort;
typedef _Atomic(int) atomic_int;
typedef _Atomic(unsigned int) atomic_uint;
typedef _Atomic(long) atomic_long;
typedef _Atomic(unsigned long) atomic_ulong;
typedef _Atomic(long long) atomic_llong;
typedef _Atomic(unsigned long long) atomic_ullong;
typedef _Atomic(__char16_t) atomic_char16_t;
typedef _Atomic(__char32_t) atomic_char32_t;
typedef _Atomic(__wchar_t) atomic_wchar_t;
typedef _Atomic(__int_least8_t) atomic_int_least8_t;
typedef _Atomic(__uint_least8_t) atomic_uint_least8_t;
typedef _Atomic(__int_least16_t) atomic_int_least16_t;
typedef _Atomic(__uint_least16_t) atomic_uint_least16_t;
typedef _Atomic(__int_least32_t) atomic_int_least32_t;
typedef _Atomic(__uint_least32_t) atomic_uint_least32_t;
typedef _Atomic(__int_least64_t) atomic_int_least64_t;
typedef _Atomic(__uint_least64_t) atomic_uint_least64_t;
typedef _Atomic(__int_fast8_t) atomic_int_fast8_t;
typedef _Atomic(__uint_fast8_t) atomic_uint_fast8_t;
typedef _Atomic(__int_fast16_t) atomic_int_fast16_t;
typedef _Atomic(__uint_fast16_t) atomic_uint_fast16_t;
typedef _Atomic(__int_fast32_t) atomic_int_fast32_t;
typedef _Atomic(__uint_fast32_t) atomic_uint_fast32_t;
typedef _Atomic(__int_fast64_t) atomic_int_fast64_t;
typedef _Atomic(__uint_fast64_t) atomic_uint_fast64_t;
typedef _Atomic(__intptr_t) atomic_intptr_t;
typedef _Atomic(__uintptr_t) atomic_uintptr_t;
typedef _Atomic(__size_t) atomic_size_t;
typedef _Atomic(__ptrdiff_t) atomic_ptrdiff_t;
typedef _Atomic(__intmax_t) atomic_intmax_t;
typedef _Atomic(__uintmax_t) atomic_uintmax_t;

#ifdef __clang__
// Clang atomic intrinsics.
#define atomic_compare_exchange_strong_explicit(object, expected, desired, \
                                                success, failure)          \
  __c11_atomic_compare_exchange_strong(object, expected, desired,          \
                                       (int)(success), (int)(failure))
#define atomic_compare_exchange_weak_explicit(object, expected, desired, \
                                              success, failure)          \
  __c11_atomic_compare_exchange_weak(object, expected, desired,          \
                                     (int)(success), (int)(failure))
#define atomic_exchange_explicit(object, desired, order) \
  __c11_atomic_exchange(object, desired, (int)(order))
#define atomic_fetch_add_explicit(object, operand, order) \
  __c11_atomic_fetch_add(object, operand, (int)(order))
#define atomic_fetch_and_explicit(object, operand, order) \
  __c11_atomic_fetch_and(object, operand, (int)(order))
#define atomic_fetch_or_explicit(object, operand, order) \
  __c11_atomic_fetch_or(object, operand, (int)(order))
#define atomic_fetch_sub_explicit(object, operand, order) \
  __c11_atomic_fetch_sub(object, operand, (int)(order))
#define atomic_fetch_xor_explicit(object, operand, order) \
  __c11_atomic_fetch_xor(object, operand, (int)(order))
#define atomic_load_explicit(object, order) \
  __c11_atomic_load(object, (int)(order))
#define atomic_store_explicit(object, desired, order) \
  __c11_atomic_store(object, desired, (int)(order))
#else
// GCC atomic intrinsics.
#define atomic_compare_exchange_strong_explicit(object, expected, desired, \
                                                success, failure)          \
  __atomic_compare_exchange_n(object, expected, desired, 0, success, failure)
#define atomic_compare_exchange_weak_explicit(object, expected, desired, \
                                              success, failure)          \
  __atomic_compare_exchange_n(object, expected, desired, 1, success, failure)
#define atomic_exchange_explicit(object, desired, order) \
  __atomic_exchange_n(object, desired, order)
#define atomic_fetch_add_explicit(object, operand, order) \
  __atomic_fetch_add(object, operand, order)
#define atomic_fetch_and_explicit(object, operand, order) \
  __atomic_fetch_and(object, operand, order)
#define atomic_fetch_or_explicit(object, operand, order) \
  __atomic_fetch_or(object, operand, order)
#define atomic_fetch_sub_explicit(object, operand, order) \
  __atomic_fetch_sub(object, operand, order)
#define atomic_fetch_xor_explicit(object, operand, order) \
  __atomic_fetch_xor(object, operand, order)
#define atomic_load_explicit(object, order) __atomic_load_n(object, order)
#define atomic_store_explicit(object, desired, order) \
  __atomic_store_n(object, desired, order)
#endif

// Variants of the above that implicitly use memory_order_seq_cst.
#define atomic_compare_exchange_strong(object, expected, desired) \
  atomic_compare_exchange_strong_explicit(                        \
      object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_weak(object, expected, desired) \
  atomic_compare_exchange_weak_explicit(                        \
      object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_exchange(object, desired) \
  atomic_exchange_explicit(object, desired, memory_order_seq_cst)
#define atomic_fetch_add(object, operand) \
  atomic_fetch_add_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_and(object, operand) \
  atomic_fetch_and_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_or(object, operand) \
  atomic_fetch_or_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_sub(object, operand) \
  atomic_fetch_sub_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_xor(object, operand) \
  atomic_fetch_xor_explicit(object, operand, memory_order_seq_cst)
#define atomic_load(object) atomic_load_explicit(object, memory_order_seq_cst)
#define atomic_store(object, desired) \
  atomic_store_explicit(object, desired, memory_order_seq_cst)

typedef struct {
  atomic_bool __value;
} atomic_flag;

__BEGIN_DECLS
void atomic_flag_clear_explicit(volatile atomic_flag *, memory_order);
void atomic_flag_clear(volatile atomic_flag *);
_Bool atomic_flag_test_and_set_explicit(volatile atomic_flag *, memory_order);
_Bool atomic_flag_test_and_set(volatile atomic_flag *);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline void __atomic_flag_clear_explicit(
    volatile atomic_flag *__object, memory_order __order) {
  atomic_store_explicit(&__object->__value, 0, __order);
}
#define atomic_flag_clear_explicit(object, order) \
  __atomic_flag_clear_explicit(object, order)

static __inline void __atomic_flag_clear(volatile atomic_flag *__object) {
  atomic_flag_clear_explicit(__object, memory_order_seq_cst);
}
#define atomic_flag_clear(object) __atomic_flag_clear(object)

static __inline _Bool __atomic_flag_test_and_set_explicit(
    volatile atomic_flag *__object, memory_order __order) {
  return atomic_exchange_explicit(&__object->__value, 1, __order);
}
#define atomic_flag_test_and_set_explicit(object, order) \
  __atomic_flag_test_and_set_explicit(object, order)

static __inline _Bool __atomic_flag_test_and_set(
    volatile atomic_flag *__object) {
  return atomic_flag_test_and_set_explicit(__object, memory_order_seq_cst);
}
#define atomic_flag_test_and_set(object) __atomic_flag_test_and_set(object)
#endif

#endif
