// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_barrier_destroy(uv_barrier_t *barrier) {
  if (pthread_barrier_destroy(barrier) != 0)
    abort();
}
