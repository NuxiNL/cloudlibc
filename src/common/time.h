// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_TIME_H
#define COMMON_TIME_H

#include <common/limits.h>
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
  cloudabi_timestamp_t s = timespec->tv_sec;
  cloudabi_timestamp_t ns = timespec->tv_nsec;
  cloudabi_timestamp_t s_max = NUMERIC_MAX(cloudabi_timestamp_t) / NSEC_PER_SEC;
  cloudabi_timestamp_t ns_max =
      NUMERIC_MAX(cloudabi_timestamp_t) % NSEC_PER_SEC;
  if (s > s_max || (s == s_max && ns > ns_max))
    return false;

  // Construct timestamp value.
  *timestamp = s * NSEC_PER_SEC + ns;
  return true;
}

static inline bool timespec_to_timestamp_clamp(
    const struct timespec *timespec, cloudabi_timestamp_t *timestamp) {
  // Invalid nanoseconds field.
  if (timespec->tv_nsec < 0 || timespec->tv_nsec >= NSEC_PER_SEC)
    return false;

  // Timestamps before the Epoch are not supported.
  if (timespec->tv_sec < 0) {
    *timestamp = 0;
    return true;
  }

  // Make sure our timestamp does not overflow.
  cloudabi_timestamp_t s = timespec->tv_sec;
  cloudabi_timestamp_t ns = timespec->tv_nsec;
  cloudabi_timestamp_t s_max = NUMERIC_MAX(cloudabi_timestamp_t) / NSEC_PER_SEC;
  cloudabi_timestamp_t ns_max =
      NUMERIC_MAX(cloudabi_timestamp_t) % NSEC_PER_SEC;
  if (s > s_max || (s == s_max && ns > ns_max)) {
    *timestamp = NUMERIC_MAX(cloudabi_timestamp_t);
    return true;
  }

  // Construct timestamp value.
  *timestamp = s * NSEC_PER_SEC + ns;
  return true;
}

static inline struct timespec timestamp_to_timespec(
    cloudabi_timestamp_t timestamp) {
  // Decompose timestamp into seconds and nanoseconds.
  return (struct timespec){.tv_sec = timestamp / NSEC_PER_SEC,
                           .tv_nsec = timestamp % NSEC_PER_SEC};
}

#endif
