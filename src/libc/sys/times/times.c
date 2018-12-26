// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/time.h>

#include <sys/times.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <cloudlibc_interceptors.h>

static_assert(CLOCKS_PER_SEC == NSEC_PER_SEC,
              "Timestamp should need no conversion");

clock_t __cloudlibc_times(struct tms *buffer) {
  // Obtain user time.
  cloudabi_timestamp_t usertime = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_PROCESS_CPUTIME_ID, 0, &usertime);
  *buffer = (struct tms){.tms_utime = usertime};

  // Obtain real time.
  cloudabi_timestamp_t realtime = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_MONOTONIC, 0, &realtime);
  return realtime;
}

__weak_reference(__cloudlibc_times, times);
