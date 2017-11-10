// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <cloudabi_types.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) {
  assert(atomic_load_explicit(&rwlock->__state, memory_order_relaxed) ==
             CLOUDABI_LOCK_UNLOCKED &&
         "Attempted to destroy locked rwlock");
  assert(rwlock->__write_recursion <= 0 && "Recursion counter invalid");
  return 0;
}

__strong_reference(pthread_rwlock_destroy, pthread_mutex_destroy);
__strong_reference(pthread_rwlock_destroy, pthread_spin_destroy);
