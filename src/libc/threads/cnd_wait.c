// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

int cnd_wait(cnd_t *cond, mtx_t *mtx) {
  if (pthread_cond_wait(cond, mtx) != 0)
    return thrd_error;
  return thrd_success;
}
