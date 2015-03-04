// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/time.h>

#include <errno.h>
#include <limits.h>

#include "time_impl.h"

static time_t modulo_remainder(time_t *numer, time_t denom) {
  time_t rem = *numer % denom;
  *numer /= denom;
  if (rem < 0) {
    rem += denom;
    --*numer;
  }
  return rem;
}

int __localtime_utc(time_t timer, struct tm *result) {
  // Compute time within the day.
  struct tm tm = {};
  tm.tm_sec = modulo_remainder(&timer, 60);
  tm.tm_min = modulo_remainder(&timer, 60);
  tm.tm_hour = modulo_remainder(&timer, 24);

  // Compute weekday. Day 0 is a Thursday.
  time_t day = timer + 4;
  tm.tm_wday = modulo_remainder(&day, 7);

  // Compute the year and the day within year by iteratively subtracting
  // the days in a year. Limit the number of iterations by making use of
  // the fact that the calendar repeats every 146097 days.
  tm.tm_yday = modulo_remainder(&timer, 146097);
  time_t year = timer * 400 + 70;
  const short *months;
  for (;;) {
    months = get_months(year);
    if (tm.tm_yday < months[12])
      break;
    tm.tm_yday -= months[12];
    ++year;
  }

  // We cannot represent this time using a "struct tm".
  if (year < INT_MIN || year > INT_MAX)
    return EOVERFLOW;
  tm.tm_year = year;

  // Compute month and day within month.
  while (tm.tm_yday >= months[tm.tm_mon + 1])
    ++tm.tm_mon;
  tm.tm_mday = tm.tm_yday - months[tm.tm_mon] + 1;

  *result = tm;
  return 0;
}
