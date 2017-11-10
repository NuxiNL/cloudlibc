// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) __no_lock_analysis {
  // The canonical case: the lock has not been acquired.
  cloudabi_lock_t old = CLOUDABI_LOCK_UNLOCKED;
  while (!atomic_compare_exchange_weak_explicit(
      &rwlock->__state, &old, old + 1,
      memory_order_acquire | __memory_order_hle_acquire,
      memory_order_relaxed)) {
    if ((old & CLOUDABI_LOCK_WRLOCKED) != 0) {
      // Another thread already has a write lock.
      assert((old & ~CLOUDABI_LOCK_KERNEL_MANAGED) !=
                 (__pthread_thread_id | CLOUDABI_LOCK_WRLOCKED) &&
             "Attempted to acquire a read lock while holding a write lock");
      return EBUSY;
    } else if ((old & CLOUDABI_LOCK_KERNEL_MANAGED) != 0 &&
               __pthread_rdlocks == 0) {
      // Lock has threads waiting for the lock. Only acquire the lock if
      // we have already acquired read locks. In that case, it is
      // justified to acquire this lock to prevent a deadlock.
      return EBUSY;
    }
  }

  // Lock acquired successfully.
  ++__pthread_rdlocks;
  return 0;
}
