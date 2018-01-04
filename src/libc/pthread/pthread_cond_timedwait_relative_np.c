// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>
#include <common/time.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <errno.h>
#include <pthread.h>

int pthread_cond_timedwait_relative_np(
    pthread_cond_t *restrict cond, __pthread_lock_t *restrict lock,
    const struct timespec *restrict abstime) {
  assert((atomic_load_explicit(&lock->__state, memory_order_relaxed) &
          ~CLOUDABI_LOCK_KERNEL_MANAGED) ==
             (__pthread_thread_id | CLOUDABI_LOCK_WRLOCKED) &&
         "This lock is not write-locked by this thread");
  assert(lock->__write_recursion <= 0 &&
         "Cannot wait using a write recursive lock");

  // Call into the kernel to wait on the condition variable.
  cloudabi_subscription_t subscriptions[2] = {
      {
          .type = CLOUDABI_EVENTTYPE_CONDVAR,
          .condvar.condvar = &cond->__waiters,
          .condvar.condvar_scope = cond->__pshared,
          .condvar.lock = &lock->__state,
          .condvar.lock_scope = lock->__pshared,
      },
      {
          .type = CLOUDABI_EVENTTYPE_CLOCK,
          .clock.clock_id = cond->__clock,
      },
  };
  if (!timespec_to_timestamp_clamp(abstime, &subscriptions[1].clock.timeout))
    return EINVAL;
  size_t triggered;

  cloudabi_event_t events[__arraycount(subscriptions)];
  cloudabi_errno_t error = cloudabi_sys_poll(
      subscriptions, events, __arraycount(subscriptions), &triggered);

  if (error != 0)
    __pthread_terminate(error, "Failed to wait on condition variable");
  for (size_t i = 0; i < triggered; ++i) {
    if (events[i].error != 0)
      __pthread_terminate(events[i].error,
                          "Failed to wait on condition variable");
    if (events[i].type == CLOUDABI_EVENTTYPE_CONDVAR)
      return 0;
  }
  return ETIMEDOUT;
}
