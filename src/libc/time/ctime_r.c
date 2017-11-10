// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

char *ctime_r(const time_t *clock, char *buf) {
  struct tm tm;
  localtime_r(clock, &tm);
  return asctime_r(&tm, buf);
}
