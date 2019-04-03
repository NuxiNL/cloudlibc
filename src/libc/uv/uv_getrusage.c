// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <uv.h>

int uv_getrusage(uv_rusage_t *rusage) {
  cloudabi_timestamp_t usertime = 0;
  (void)cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_PROCESS_CPUTIME_ID, 1000,
                                    &usertime);
  *rusage = (uv_rusage_t){
      .ru_utime.tv_sec = usertime / 1000000000,
      .ru_utime.tv_usec = (usertime % 1000000000) / 1000,
  };
  return 0;
}
