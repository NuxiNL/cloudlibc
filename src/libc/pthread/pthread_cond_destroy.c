// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <cloudabi_types.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_cond_destroy(pthread_cond_t *cond) {
  assert(atomic_load_explicit(&cond->__waiters, memory_order_relaxed) ==
             CLOUDABI_CONDVAR_HAS_NO_WAITERS &&
         "Attempted to destroy a condition variable with blocked threads");
  return 0;
}
