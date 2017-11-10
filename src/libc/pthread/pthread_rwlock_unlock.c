// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) __no_lock_analysis {
  _Atomic(cloudabi_lock_t) *state = &rwlock->__state;
  cloudabi_lock_t old = atomic_load_explicit(state, memory_order_relaxed);
  if ((old & CLOUDABI_LOCK_WRLOCKED) != 0) {
    assert((old & ~CLOUDABI_LOCK_KERNEL_MANAGED) ==
               (__pthread_thread_id | CLOUDABI_LOCK_WRLOCKED) &&
           "This rwlock is write-locked by a different thread");

    // Perform a write unlock. If the lock has been acquired
    // recursively, only decrement the recursion counter without
    // actually unlocking.
    if (rwlock->__write_recursion > 0) {
      --rwlock->__write_recursion;
      return 0;
    }

    // Attempt to unlock from userspace.
    if ((old & CLOUDABI_LOCK_KERNEL_MANAGED) != 0 ||
        !atomic_compare_exchange_strong_explicit(
            state, &old, CLOUDABI_LOCK_UNLOCKED,
            memory_order_release | __memory_order_hle_release,
            memory_order_relaxed)) {
      // Lock is managed by kernelspace. Call into the kernel to unblock
      // waiting threads.
      cloudabi_errno_t error =
          cloudabi_sys_lock_unlock(state, rwlock->__pshared);
      if (error != 0)
        __pthread_terminate(error, "Failed to write unlock a rwlock");
    }
  } else {
    assert(__pthread_rdlocks > 0 && "Bad lock count");
    assert(rwlock->__write_recursion <= 0 &&
           "Write recursion field does not match lock value");

    // Perform a read unlock. We can do this in userspace, except when
    // other threads are blocked and we are performing the last unlock.
    // In that case, call into the kernel.
    //
    // Other threads may attempt to increment the read lock count,
    // meaning that the call into the kernel could be spurious. To
    // prevent this from happening, upgrade to a write lock first. This
    // allows us to call into the kernel, having the guarantee that the
    // lock value will not change in the meantime.
    for (;;) {
      if (old == (1 | CLOUDABI_LOCK_KERNEL_MANAGED)) {
        // Last read lock while threads are waiting. Attempt to upgrade
        // to a write lock before calling into the kernel to unlock.
        if (atomic_compare_exchange_weak_explicit(
                state, &old,
                __pthread_thread_id | CLOUDABI_LOCK_WRLOCKED |
                    CLOUDABI_LOCK_KERNEL_MANAGED,
                memory_order_release | __memory_order_hle_release,
                memory_order_relaxed)) {
          // Call into the kernel to unlock.
          cloudabi_errno_t error =
              cloudabi_sys_lock_unlock(state, rwlock->__pshared);
          if (error != 0)
            __pthread_terminate(error, "Failed to read unlock a rwlock");
          break;
        }
      } else {
        // No threads waiting or not the last read lock. Just decrement
        // the read lock count.
        assert((old & ~CLOUDABI_LOCK_KERNEL_MANAGED) != 0 &&
               "This rwlock is not locked");
        if (atomic_compare_exchange_weak_explicit(
                state, &old, old - 1,
                memory_order_release | __memory_order_hle_release,
                memory_order_relaxed))
          break;
      }
    }

    --__pthread_rdlocks;
  }
  return 0;
}

__strong_reference(pthread_rwlock_unlock, pthread_mutex_unlock);
__strong_reference(pthread_rwlock_unlock, pthread_spin_unlock);
