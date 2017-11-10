// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef STDATOMIC_STDATOMIC_IMPL_H
#define STDATOMIC_STDATOMIC_IMPL_H

#include <sys/types.h>

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

// Clang doesn't allow us to explicitly implement builtin functions.
// Work around this limitation by emitting a function with a different
// name and instructing Clang to do a rename.
#ifdef __clang__
#pragma redefine_extname atomic_compare_exchange_fallback \
    __atomic_compare_exchange
#pragma redefine_extname atomic_exchange_fallback __atomic_exchange
#pragma redefine_extname atomic_load_fallback __atomic_load
#pragma redefine_extname atomic_store_fallback __atomic_store
#define __atomic_compare_exchange atomic_compare_exchange_fallback
#define __atomic_exchange atomic_exchange_fallback
#define __atomic_load atomic_load_fallback
#define __atomic_store atomic_store_fallback
#endif

// Intrinsics that are being called for objects that are too large for
// the CPU to do natively.
bool __atomic_compare_exchange(size_t, void *, void *, void *, memory_order,
                               memory_order);
void __atomic_exchange(size_t, void *, void *, void *, memory_order);
void __atomic_load(size_t, void *, void *, memory_order);
void __atomic_store(size_t, void *, void *, memory_order);

// Exponent of the size of the table of locks that should be used for
// atomic objects that are not lock-free.
#define ATOMIC_FALLBACK_NBITS 4

// Table of locks.
extern pthread_rwlock_t __atomic_fallback_locks[];

// Fetches a lock for an atomic object that is not lock-free, based on
// the address of the object.
static inline pthread_rwlock_t *atomic_fallback_getlock(void *object) {
  if (ATOMIC_FALLBACK_NBITS == 0) {
    // Table consists of a single lock. Return lock without performing
    // any arithmetic.
    return &__atomic_fallback_locks[0];
  } else {
    // Use Knuth's multiplicative method to hash the address.
    uint32_t v = (uintptr_t)object;
    return &__atomic_fallback_locks[v * UINT32_C(2654435761) >>
                                    (32 - ATOMIC_FALLBACK_NBITS)];
  }
}

#endif
