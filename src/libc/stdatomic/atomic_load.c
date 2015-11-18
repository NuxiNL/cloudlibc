// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <string.h>

#include "stdatomic_impl.h"

void __atomic_load(size_t len, void *object, void *value, memory_order order) {
  pthread_rwlock_t *lock = atomic_fallback_getlock(object);
  pthread_rwlock_rdlock(lock);
  memcpy(value, object, len);
  pthread_rwlock_unlock(lock);
}
