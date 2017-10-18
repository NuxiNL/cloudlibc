// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_rwlock_wrlock(uv_rwlock_t *rwlock) {
  if (pthread_rwlock_wrlock(rwlock) != 0)
    abort();
}

__strong_reference(uv_rwlock_wrlock, uv_mutex_lock);
