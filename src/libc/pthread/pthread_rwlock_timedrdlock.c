// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>
#include <common/syscalls.h>
#include <common/time.h>

#include <errno.h>
#include <pthread.h>

int pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock,
                               const struct timespec *restrict
                                   abstime) __no_lock_analysis {
  // Attempt to acquire the lock in userspace.
  for (int i = 0; i < LOCK_RETRY_COUNT; ++i) {
    int error = pthread_rwlock_tryrdlock(rwlock);
    if (error != EBUSY)
      return error;
  }

  // Call into the kernel to acquire a read lock.
  cloudabi_event_t events[2] = {
      {
          .type = CLOUDABI_EVENT_TYPE_LOCK_RDLOCK,
          .lock.lock = &rwlock->__state,
      },
      {
          .type = CLOUDABI_EVENT_TYPE_CLOCK, .clock.clock_id = CLOCK_REALTIME,
      },
  };
  if (!timespec_to_timestamp_clamp(abstime, &events[1].clock.timeout))
    return EINVAL;

  size_t triggered;
  cloudabi_errno_t error =
      cloudabi_sys_poll_once(events, 2, events, 2, &triggered);
  if (error != 0)
    __pthread_terminate(error, "Failed to acquire read lock");
  for (size_t i = 0; i < triggered; ++i) {
    if (events[i].type == CLOUDABI_EVENT_TYPE_LOCK_RDLOCK) {
      if (events[i].error != 0)
        __pthread_terminate(events[i].error, "Failed to acquire read lock");

      // Lock acquired successfully.
      ++__pthread_rdlocks;
      return 0;
    }
  }
  return ETIMEDOUT;
}
