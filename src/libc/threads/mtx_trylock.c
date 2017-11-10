// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>
#include <threads.h>

int mtx_trylock(mtx_t *mtx) {
  switch (pthread_mutex_trylock(mtx)) {
    case 0:
      return thrd_success;
    case EBUSY:
      return thrd_busy;
    default:
      return thrd_error;
  }
}
