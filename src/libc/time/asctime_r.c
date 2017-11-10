// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

char *asctime_r(const struct tm *restrict tm, char *restrict buf) {
  return strftime(buf, 26, "%c\n", tm) == 0 ? NULL : buf;
}
