// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

int uv_barrier_wait(uv_barrier_t *barrier) {
  int error = pthread_barrier_wait(barrier);
  switch (error) {
    case 0:
      return 0;
    case PTHREAD_BARRIER_SERIAL_THREAD:
      return 1;
    default:
      return -error;
  }
}
