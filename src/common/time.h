// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_TIME_H
#define COMMON_TIME_H

#include <common/limits.h>
#include <common/overflow.h>
#include <common/syscalldefs.h>

#include <stdbool.h>
#include <time.h>

#define NSEC_PER_SEC 1000000000

static inline bool is_leap(time_t year) {
  year %= 400;
  if (year < 0)
    year += 400;
  return ((year % 4) == 0 && (year % 100) != 0) || year == 100;
}

// Gets the cumulative length of the months in a year.
static inline const short *get_months(time_t year) {
  static const short leap[13] = {0,   31,  60,  91,  121, 152, 182,
                                 213, 244, 274, 305, 335, 366};
  static const short common[13] = {0,   31,  59,  90,  120, 151, 181,
                                   212, 243, 273, 304, 334, 365};
  return is_leap(year) ? leap : common;
}

static inline short get_ydays(time_t year) {
  return is_leap(year) ? 366 : 365;
}

static inline bool timespec_to_timestamp_exact(
    const struct timespec *timespec, cloudabi_timestamp_t *timestamp) {
  // Invalid nanoseconds field.
  if (timespec->tv_nsec < 0 || timespec->tv_nsec >= NSEC_PER_SEC)
    return false;

  // Timestamps before the Epoch are not supported.
  if (timespec->tv_sec < 0)
    return false;

  // Make sure our timestamp does not overflow.
  return !mul_overflow(timespec->tv_sec, NSEC_PER_SEC, timestamp) &&
         !add_overflow(*timestamp, timespec->tv_nsec, timestamp);
}

static inline bool timespec_to_timestamp_clamp(
    const struct timespec *timespec, cloudabi_timestamp_t *timestamp) {
  // Invalid nanoseconds field.
  if (timespec->tv_nsec < 0 || timespec->tv_nsec >= NSEC_PER_SEC)
    return false;

  if (timespec->tv_sec < 0) {
    // Timestamps before the Epoch are not supported.
    *timestamp = 0;
  } else if (mul_overflow(timespec->tv_sec, NSEC_PER_SEC, timestamp) ||
             add_overflow(*timestamp, timespec->tv_nsec, timestamp)) {
    // Make sure our timestamp does not overflow.
    *timestamp = NUMERIC_MAX(cloudabi_timestamp_t);
  }
  return true;
}

static inline struct timespec timestamp_to_timespec(
    cloudabi_timestamp_t timestamp) {
  // Decompose timestamp into seconds and nanoseconds.
  return (struct timespec){.tv_sec = timestamp / NSEC_PER_SEC,
                           .tv_nsec = timestamp % NSEC_PER_SEC};
}

#endif
