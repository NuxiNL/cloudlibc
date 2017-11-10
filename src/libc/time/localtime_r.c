// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <time.h>

#include "time_impl.h"

struct tm *localtime_r(const time_t *restrict timer,
                       struct tm *restrict result) {
  int error = __localtime_utc(*timer, result);
  result->tm_zone = "UTC";
  if (error != 0) {
    errno = error;
    return NULL;
  }
  return result;
}

__strong_reference(localtime_r, gmtime_r);
