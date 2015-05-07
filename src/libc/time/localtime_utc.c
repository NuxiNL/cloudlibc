// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/time.h>

#include <errno.h>
#include <limits.h>

#include "time_impl.h"

#define ERALENGTH ((time_t)146097 * 24 * 60 * 60)

int __localtime_utc(time_t timer, struct tm *result) {
  // The calendar repeats every 146097 days (400 years). Normalize the
  // timer to lie within this range. This ensures that the timestamp is
  // positive, simplifying the arithmetic below.
  time_t era = timer / ERALENGTH;
  timer %= ERALENGTH;
  if (timer < 0) {
    timer += ERALENGTH;
    --era;
  }

  // Compute time within the day.
  struct tm tm = {};
  tm.tm_sec = timer % 60;
  timer /= 60;
  tm.tm_min = timer % 60;
  timer /= 60;
  tm.tm_hour = timer % 24;
  timer /= 24;

  // Compute weekday. Day 0 is a Thursday.
  tm.tm_wday = (timer + 4) % 7;

  // Apply leap days, so that every year becomes exactly 366 days long.
  // First add the three leap days at the boundaries of the centuries.
  // After that we can add up to three leap days per 1461 days.
  timer += (timer - 11323) / 36524;
  timer += timer / 1461 + (timer + 731) / 1461 + (timer + 1096) / 1461;
  tm.tm_yday = timer % 366;

  // Compute the year. "struct tm" only uses an integer to hold the year
  // number. Bail out if the year number doesn't fit.
  time_t year = era * 400 + timer / 366 + 70;
  if (year < INT_MIN || year > INT_MAX)
    return EOVERFLOW;
  tm.tm_year = year;

  // Compute month and day within month.
  const short *months = get_months(year);
  while (tm.tm_yday >= months[tm.tm_mon + 1])
    ++tm.tm_mon;
  tm.tm_mday = tm.tm_yday - months[tm.tm_mon] + 1;

  *result = tm;
  return 0;
}
