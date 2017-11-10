// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <errno.h>
#include <pthread.h>

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) __no_lock_analysis {
  // Attempt to acquire the lock in userspace.
  for (int i = 0; i < LOCK_RETRY_COUNT; ++i) {
    int error = pthread_rwlock_trywrlock(rwlock);
    if (error != EBUSY)
      return error;
  }

  // Call into the kernel to acquire a write lock.
  cloudabi_subscription_t subscription = {
      .type = CLOUDABI_EVENTTYPE_LOCK_WRLOCK,
      .lock.lock = &rwlock->__state,
      .lock.lock_scope = rwlock->__pshared,
  };
  size_t triggered;
  cloudabi_event_t event;
  cloudabi_errno_t error =
      cloudabi_sys_poll(&subscription, &event, 1, &triggered);
  if (error != 0)
    __pthread_terminate(error, "Failed to acquire write lock");
  if (event.error != 0)
    __pthread_terminate(event.error, "Failed to acquire write lock");
  assert(rwlock->__write_recursion <= 0 && "Invalid write recursion count");

  // Lock acquired successfully.
  return 0;
}

__strong_reference(pthread_rwlock_wrlock, pthread_mutex_lock);
__strong_reference(pthread_rwlock_wrlock, pthread_spin_lock);
