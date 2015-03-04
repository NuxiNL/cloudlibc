// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/time.h>

#include "time_impl.h"

static time_t modulo_quotient(time_t *numer, time_t denom) {
  time_t quot = *numer / denom;
  *numer %= denom;
  if (*numer < 0) {
    *numer += denom;
    --quot;
  }
  return quot;
}

void __mktime_utc(const struct tm *tm, struct timespec *result) {
  // Normalize nanoseconds.
  time_t nsec = tm->tm_nsec;
  time_t sec = (time_t)tm->tm_sec + (time_t)tm->tm_min * 60 +
               (time_t)tm->tm_hour * 3600 +
               modulo_quotient(&nsec, NSEC_PER_SEC);

  // Normalize and eliminate months.
  time_t mon = tm->tm_mon;
  time_t year = tm->tm_year + modulo_quotient(&mon, 12);
  time_t yday = (time_t)tm->tm_mday + get_months(year)[mon] - 1;

  // We need to ensure that the year is at least 70 for the rounding in
  // the day computation to work properly. Temporarily normalize the
  // year number by using an additional era counter.
  time_t era = 0;
  era += year / 400 - 2;
  year = year % 400 + 800;

  // Compute days since epoch.
  time_t day = yday + (year - 70) * 365 + (year - 69) / 4 - (year - 1) / 100 +
               (year + 299) / 400 + era * 146097;

  // Merge results together.
  *result = (struct timespec){.tv_sec = sec + day * 86400, .tv_nsec = nsec};
}
