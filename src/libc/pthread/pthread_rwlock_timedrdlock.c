// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>
#include <common/time.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <pthread.h>

int pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock,
                               const struct timespec *restrict abstime)
    __no_lock_analysis {
  // Attempt to acquire the lock in userspace.
  for (int i = 0; i < LOCK_RETRY_COUNT; ++i) {
    int error = pthread_rwlock_tryrdlock(rwlock);
    if (error != EBUSY)
      return error;
  }

  // Call into the kernel to acquire a read lock.
  cloudabi_subscription_t subscriptions[2] = {
      {
          .type = CLOUDABI_EVENTTYPE_LOCK_RDLOCK,
          .lock.lock = &rwlock->__state,
          .lock.lock_scope = rwlock->__pshared,
      },
      {
          .type = CLOUDABI_EVENTTYPE_CLOCK,
          .clock.clock_id = CLOUDABI_CLOCK_REALTIME,
          .clock.flags = CLOUDABI_SUBSCRIPTION_CLOCK_ABSTIME,
      },
  };
  if (!timespec_to_timestamp_clamp(abstime, &subscriptions[1].clock.timeout))
    return EINVAL;

  size_t triggered;
  cloudabi_event_t events[__arraycount(subscriptions)];
  cloudabi_errno_t error = cloudabi_sys_poll(
      subscriptions, events, __arraycount(subscriptions), &triggered);
  if (error != 0)
    __pthread_terminate(error, "Failed to acquire read lock");
  for (size_t i = 0; i < triggered; ++i) {
    if (events[i].error != 0)
      __pthread_terminate(events[i].error, "Failed to acquire read lock");
    if (events[i].type == CLOUDABI_EVENTTYPE_LOCK_RDLOCK) {
      // Lock acquired successfully.
      ++__pthread_rdlocks;
      return 0;
    }
  }
  return ETIMEDOUT;
}
