// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <pthread.h>
#include <stdatomic.h>

#undef pthread_once

int pthread_once(pthread_once_t *once_control, void (*init_routine)(void)) {
  // Implement once objects by abusing the infrastructure for read-write
  // locks.
  //
  // The thread executing the initialization routine will hold an
  // exclusive lock. If other threads need to block, they will pick up a
  // read lock. Once finished, the once object will either be unlocked
  // or will have some stale read locks. To distinguish between the
  // initial state and the final state, the initial state uses
  // CLOUDABI_LOCK_BOGUS.
  //
  // By reusing read-write locks, the kernel may apply priority
  // inheritance.
  cloudabi_lock_t expected = CLOUDABI_LOCK_BOGUS;
  cloudabi_lock_t locked = __pthread_thread_id | CLOUDABI_LOCK_WRLOCKED;
  if (atomic_compare_exchange_strong_explicit(
          &once_control->__state, &expected, locked,
          memory_order_acquire | __memory_order_hle_acquire,
          memory_order_acquire)) {
    // First invocation. Execute initialization routine.
    init_routine();

    // Attempt to mark the once object as finished.
    if (!atomic_compare_exchange_strong_explicit(
            &once_control->__state, &locked, CLOUDABI_LOCK_UNLOCKED,
            memory_order_release | __memory_order_hle_release,
            memory_order_relaxed)) {
      assert(locked == (__pthread_thread_id | CLOUDABI_LOCK_WRLOCKED |
                        CLOUDABI_LOCK_KERNEL_MANAGED) &&
             "Corrupt once object");

      // Once object has other threads waiting on its completion. Call into the
      // kernel to unblock the waiting threads.
      cloudabi_errno_t error = cloudabi_sys_lock_unlock(&once_control->__state,
                                                        CLOUDABI_SCOPE_PRIVATE);
      if (error != 0)
        __pthread_terminate(error,
                            "Failed to wake up threads blocked on once object");
    }
  } else if ((expected & CLOUDABI_LOCK_WRLOCKED) != 0) {
    // Initialization routine is being executed. Suspend execution of
    // this thread until the other thread has finished executing the
    // initialization routine.
    cloudabi_subscription_t subscription = {
        .type = CLOUDABI_EVENTTYPE_LOCK_RDLOCK,
        .lock.lock = &once_control->__state,
        .lock.lock_scope = CLOUDABI_SCOPE_PRIVATE,
    };
    size_t triggered;
    cloudabi_event_t event;
    cloudabi_errno_t error =
        cloudabi_sys_poll(&subscription, &event, 1, &triggered);
    if (error != 0)
      __pthread_terminate(error, "Failed to wait on once object");
    if (event.error != 0)
      __pthread_terminate(event.error, "Failed to wait on once object");
  }

  // After unblocking, there may still be a number of 'read-lockers'.
  // Explictly set the lock to the initial state to make the fast path work.
  atomic_store_explicit(&once_control->__state, CLOUDABI_LOCK_UNLOCKED,
                        memory_order_relaxed);
  return 0;
}
