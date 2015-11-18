// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <threads.h>

int mtx_lock(mtx_t *mtx) {
  if (pthread_mutex_lock(mtx) != 0)
    return thrd_error;
  return thrd_success;
}
