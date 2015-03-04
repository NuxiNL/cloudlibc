// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <string.h>

#include "stdatomic_impl.h"

void __atomic_load(size_t len, void *object, void *value, memory_order order) {
  pthread_rwlock_rdlock(&__atomic_fallback_lock);
  memcpy(value, object, len);
  pthread_rwlock_unlock(&__atomic_fallback_lock);
}
