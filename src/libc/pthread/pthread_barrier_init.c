// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_types.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_barrier_init(pthread_barrier_t *restrict barrier,
                         const pthread_barrierattr_t *restrict attr,
                         unsigned int count) {
  // Barrier with zero threads is meaningless.
  if (count == 0)
    return EINVAL;

  // Initialize lock.
  atomic_init(&barrier->__lock.__state, CLOUDABI_LOCK_UNLOCKED);
  barrier->__lock.__write_recursion = -1;
  barrier->__lock.__pshared =
      attr != NULL ? attr->__pshared : PTHREAD_PROCESS_PRIVATE;

  // Initialize condition variable.
  atomic_init(&barrier->__cond.__waiters, CLOUDABI_CONDVAR_HAS_NO_WAITERS);
  barrier->__cond.__pshared =
      attr != NULL ? attr->__pshared : PTHREAD_PROCESS_PRIVATE;

  // Initialize other fields.
  barrier->__init = count - 1;
  barrier->__remaining = count - 1;
  barrier->__generation = 0;
  return 0;
}
