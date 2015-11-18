// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>
#include <threads.h>

int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mtx,
                  const struct timespec *restrict ts) {
  switch (pthread_cond_timedwait(cond, mtx, ts)) {
    case 0:
      return thrd_success;
    case ETIMEDOUT:
      return thrd_timedout;
    default:
      return thrd_error;
  }
}
