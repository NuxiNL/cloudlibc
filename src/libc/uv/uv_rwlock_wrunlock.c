// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_rwlock_wrunlock(uv_rwlock_t *rwlock) {
  if (pthread_rwlock_unlock(rwlock) != 0)
    abort();
}

__strong_reference(uv_rwlock_wrunlock, uv_mutex_unlock);
__strong_reference(uv_rwlock_wrunlock, uv_rwlock_rdunlock);
