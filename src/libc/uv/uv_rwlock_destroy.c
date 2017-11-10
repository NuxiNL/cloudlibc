// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_rwlock_destroy(uv_rwlock_t *rwlock) {
  if (pthread_rwlock_destroy(rwlock) != 0)
    abort();
}

__strong_reference(uv_rwlock_destroy, uv_mutex_destroy);
