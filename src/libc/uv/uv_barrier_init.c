// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <uv.h>

int uv_barrier_init(uv_barrier_t *barrier, unsigned int count) {
  return -pthread_barrier_init(barrier, NULL, count);
}
