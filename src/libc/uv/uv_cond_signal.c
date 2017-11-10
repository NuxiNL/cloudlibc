// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_cond_signal(uv_cond_t *cond) {
  if (pthread_cond_signal(cond) != 0)
    abort();
}
