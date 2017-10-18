// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <uv.h>

int uv_rwlock_trywrlock(uv_rwlock_t *rwlock) {
  return -pthread_rwlock_trywrlock(rwlock);
}

__strong_reference(uv_rwlock_trywrlock, uv_mutex_trylock);
