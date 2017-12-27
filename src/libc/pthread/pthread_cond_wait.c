// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <pthread.h>

int pthread_cond_wait(pthread_cond_t *restrict cond,
                      __pthread_lock_t *restrict lock) {
  assert((atomic_load_explicit(&lock->__state, memory_order_relaxed) &
          ~CLOUDABI_LOCK_KERNEL_MANAGED) ==
             (__pthread_thread_id | CLOUDABI_LOCK_WRLOCKED) &&
         "This lock is not write-locked by this thread");
  assert(lock->__write_recursion <= 0 &&
         "Cannot wait using a write recursive lock");

  // Call into the kernel to wait on the condition variable.
  cloudabi_subscription_t subscription = {
      .type = CLOUDABI_EVENTTYPE_CONDVAR,
      .condvar.condvar = &cond->__waiters,
      .condvar.condvar_scope = cond->__pshared,
      .condvar.lock = &lock->__state,
      .condvar.lock_scope = lock->__pshared,
  };
  size_t triggered;

  cloudabi_event_t event;
  cloudabi_errno_t error =
      cloudabi_sys_poll(&subscription, &event, 1, &triggered);

  if (error != 0)
    __pthread_terminate(error, "Failed to wait on condition variable");
  if (event.error != 0)
    __pthread_terminate(event.error, "Failed to wait on condition variable");

  return 0;
}
