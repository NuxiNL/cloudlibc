// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <threads.h>
#include <time.h>

int nanosleep(const struct timespec *rqtp, ...) {
  int error = clock_nanosleep(CLOCK_REALTIME, 0, rqtp);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return 0;
}

__strong_reference(nanosleep, thrd_sleep);
