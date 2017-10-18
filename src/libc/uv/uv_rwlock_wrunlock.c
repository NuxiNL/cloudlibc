// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_rwlock_wrunlock(uv_rwlock_t *rwlock) {
  if (pthread_rwlock_unlock(rwlock) != 0)
    abort();
}

__strong_reference(uv_rwlock_wrunlock, uv_mutex_unlock);
__strong_reference(uv_rwlock_wrunlock, uv_rwlock_rdunlock);
