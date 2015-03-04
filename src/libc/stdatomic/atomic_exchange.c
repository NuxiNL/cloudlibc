// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <string.h>

#include "stdatomic_impl.h"

void __atomic_exchange(size_t len, void *object, void *new, void *old,
                       memory_order order) {
  pthread_rwlock_wrlock(&__atomic_fallback_lock);
  memcpy(old, object, len);
  memcpy(object, new, len);
  pthread_rwlock_unlock(&__atomic_fallback_lock);
}
