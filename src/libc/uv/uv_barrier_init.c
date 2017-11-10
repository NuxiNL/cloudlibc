// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_barrier_init(uv_barrier_t *barrier, unsigned int count) {
  return -pthread_barrier_init(barrier, NULL, count);
}
