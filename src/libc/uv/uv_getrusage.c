// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <uv.h>

int uv_getrusage(uv_rusage_t *rusage) {
  cloudabi_timestamp_t usertime = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_PROCESS_CPUTIME_ID, 1000,
                              &usertime);
  *rusage = (uv_rusage_t){
      .ru_utime.tv_sec = usertime / 1000000000,
      .ru_utime.tv_usec = (usertime % 1000000000) / 1000,
  };
  return 0;
}
