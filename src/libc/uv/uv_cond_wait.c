// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_cond_wait(uv_cond_t *cond, uv_mutex_t *mutex) {
  if (pthread_cond_wait(cond, mutex) != 0)
    abort();
}
