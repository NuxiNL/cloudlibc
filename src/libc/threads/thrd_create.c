// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>
#include <threads.h>

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
  switch (pthread_create(thr, NULL, (void *(*)(void *))func, arg)) {
    case 0:
      return thrd_success;
    case ENOMEM:
      return thrd_nomem;
    default:
      return thrd_error;
  }
}
