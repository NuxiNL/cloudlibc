// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <time.h>
#include <unistd.h>

int usleep(useconds_t useconds) {
  struct timespec ts = {.tv_sec = useconds / 1000000,
                        .tv_nsec = useconds % 1000000 * 1000};
  int error = clock_nanosleep(CLOCK_REALTIME, 0, &ts);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return 0;
}
