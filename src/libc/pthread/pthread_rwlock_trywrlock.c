// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/pthread.h>

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock) __no_lock_analysis {
  // The canonical case: the lock has not been acquired.
  cloudabi_lock_t old = CLOUDABI_LOCK_UNLOCKED;
  if (atomic_compare_exchange_strong_explicit(
          &rwlock->__state, &old, __pthread_thread_id | CLOUDABI_LOCK_WRLOCKED,
          memory_order_acquire, memory_order_relaxed)) {
    LIST_INSERT_HEAD(&__pthread_wrlocks, rwlock, __write_locks);
    return 0;
  }

  // If we fail to acquire the lock, it may be the case that we've
  // already acquired it and may need to recurse.
  if ((old & ~CLOUDABI_LOCK_KERNEL_MANAGED) ==
      (__pthread_thread_id | CLOUDABI_LOCK_WRLOCKED)) {
    assert(rwlock->__write_recursion >= 0 &&
           "Attempted to recursively write-lock a non-recursive rwlock");
    assert(!LIST_EMPTY(&__pthread_wrlocks) && "Bad write-lock count");
    ++rwlock->__write_recursion;
    return 0;
  }

  // Lock cannot be acquired without blocking.
  return EBUSY;
}

__strong_reference(pthread_rwlock_trywrlock, pthread_mutex_trylock);
__strong_reference(pthread_rwlock_trywrlock, pthread_spin_trylock);
