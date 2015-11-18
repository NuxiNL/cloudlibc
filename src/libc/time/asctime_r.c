// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <time.h>

char *asctime_r(const struct tm *restrict tm, char *restrict buf) {
  return strftime(buf, 26, "%c\n", tm) == 0 ? NULL : buf;
}
