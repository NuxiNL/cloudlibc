// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_rwlock_tryrdlock(uv_rwlock_t *rwlock) {
  return -pthread_rwlock_tryrdlock(rwlock);
}
