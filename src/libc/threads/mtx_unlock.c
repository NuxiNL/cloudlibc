// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

int mtx_unlock(mtx_t *mtx) {
  if (pthread_mutex_unlock(mtx) != 0)
    return thrd_error;
  return thrd_success;
}
