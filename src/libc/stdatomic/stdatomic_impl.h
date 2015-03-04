// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef STDATOMIC_STDATOMIC_IMPL_H
#define STDATOMIC_STDATOMIC_IMPL_H

#include <sys/types.h>

#include <stdatomic.h>
#include <stdbool.h>

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

// Lock to serialize all calls. We should consider changing this to a
// pool of locks and hashing the address of the object.
extern pthread_rwlock_t __atomic_fallback_lock;

#endif
