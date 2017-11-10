// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <string.h>

#include "stdatomic_impl.h"

bool __atomic_compare_exchange(size_t len, void *object, void *expected,
                               void *desired, memory_order success,
                               memory_order failure) {
  pthread_rwlock_t *lock = atomic_fallback_getlock(object);
  pthread_rwlock_wrlock(lock);
  if (memcmp(object, expected, len) == 0) {
    memcpy(object, desired, len);
    pthread_rwlock_unlock(lock);
    return true;
  } else {
    memcpy(expected, object, len);
    pthread_rwlock_unlock(lock);
    return false;
  }
}
