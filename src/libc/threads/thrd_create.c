// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

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
