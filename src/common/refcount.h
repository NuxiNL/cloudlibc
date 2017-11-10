// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_REFCOUNT_H
#define COMMON_REFCOUNT_H

#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>

typedef struct {
  atomic_uint count;
} refcount_t;

#define REFCOUNT_INIT(count) \
  { ATOMIC_VAR_INIT(count) }

// Initializes a reference counter.
static inline void refcount_init(refcount_t *refcount, unsigned int count) {
  atomic_init(&refcount->count, count);
}

// Acquires the reference counter.
static inline void refcount_acquire(refcount_t *refcount) {
#ifdef NDEBUG
  atomic_fetch_add_explicit(&refcount->count, 1, memory_order_acquire);
#else
  unsigned int old =
      atomic_fetch_add_explicit(&refcount->count, 1, memory_order_acquire);
  assert(old > 0 && "Attempted to acquire unreferenced object");
#endif
}

// Release a reference count object.
static inline bool refcount_release(refcount_t *refcount) {
  return atomic_fetch_sub_explicit(&refcount->count, 1, memory_order_release) ==
         1;
}

// Assert that the reference count object is not referenced multiple times.
static inline void refcount_assert_exclusive(refcount_t *refcount) {
  assert(atomic_load_explicit(&refcount->count, memory_order_acquire) <= 1 &&
         "Object is still referenced multiple times");
}

#endif
