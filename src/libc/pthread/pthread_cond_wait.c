// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/pthread.h>
#include <common/syscalls.h>

#include <assert.h>
#include <pthread.h>

int pthread_cond_wait(pthread_cond_t *restrict cond,
                      __pthread_lock_t *restrict lock) {
  assert(!LIST_EMPTY(&__pthread_wrlocks) && "Bad lock count");
  assert((atomic_load_explicit(&lock->__state, memory_order_relaxed) &
          ~CLOUDABI_LOCK_KERNEL_MANAGED) ==
             (__pthread_thread_id | CLOUDABI_LOCK_WRLOCKED) &&
         "This lock is not write-locked by this thread");
  assert(lock->__write_recursion <= 0 &&
         "Cannot wait using a write recursive lock");

  // Call into the kernel to wait on the condition variable.
  cloudabi_event_t event = {
      .type = CLOUDABI_EVENT_TYPE_CONDVAR,
      .condvar.condvar = &cond->__waiters,
      .condvar.lock = &lock->__state,
  };
  size_t triggered;

  // Remove lock from lock list while blocking.
  LIST_REMOVE(lock, __write_locks);
  cloudabi_errno_t error =
      cloudabi_sys_poll_once(&event, 1, &event, 1, &triggered);
  LIST_INSERT_HEAD(&__pthread_wrlocks, lock, __write_locks);

  if (error != 0)
    __pthread_terminate(error, "Failed to wait on condition variable");
  if (event.error != 0)
    __pthread_terminate(event.error, "Failed to wait on condition variable");

  return 0;
}
