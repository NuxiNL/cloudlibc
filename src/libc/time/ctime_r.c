// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <time.h>

char *ctime_r(const time_t *clock, char *buf) {
  struct tm tm;
  localtime_r(clock, &tm);
  return asctime_r(&tm, buf);
}
