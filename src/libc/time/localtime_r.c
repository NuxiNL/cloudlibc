// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <string.h>
#include <time.h>

#include "time_impl.h"

struct tm *localtime_r(const time_t *restrict timer,
                       struct tm *restrict result) {
  int error = __localtime_utc(*timer, result);
  if (error != 0) {
    errno = error;
    return NULL;
  }
  strlcpy(result->tm_zone, "UTC", sizeof(result->tm_zone));
  return result;
}

__strong_reference(localtime_r, gmtime_r);
