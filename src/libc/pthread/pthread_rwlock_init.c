// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalldefs.h>

#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>

static_assert(PTHREAD_PROCESS_PRIVATE == CLOUDABI_MAP_PRIVATE,
              "Value mismatch");
static_assert(PTHREAD_PROCESS_SHARED == CLOUDABI_MAP_SHARED, "Value mismatch");

int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
                        const pthread_rwlockattr_t *restrict attr) {
  atomic_init(&rwlock->__state, CLOUDABI_LOCK_UNLOCKED);
  rwlock->__write_recursion =
      attr != NULL && attr->__type == PTHREAD_MUTEX_RECURSIVE ? 0 : -1;
  rwlock->__pshared = attr != NULL ? attr->__pshared : PTHREAD_PROCESS_PRIVATE;
  return 0;
}

__strong_reference(pthread_rwlock_init, pthread_mutex_init);
