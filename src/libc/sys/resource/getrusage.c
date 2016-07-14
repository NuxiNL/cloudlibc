// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/time.h>

#include <sys/resource.h>

#include <cloudabi_syscalls.h>
#include <errno.h>

int getrusage(int who, struct rusage *r_usage) {
  switch (who) {
    case RUSAGE_SELF: {
      cloudabi_timestamp_t usertime = 0;
      cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_PROCESS_CPUTIME_ID, 1000,
                                  &usertime);
      *r_usage = (struct rusage){
          .ru_utime.tv_sec = usertime / NSEC_PER_SEC,
          .ru_utime.tv_usec = (usertime % NSEC_PER_SEC) / 1000,
      };
      return 0;
    }
    case RUSAGE_CHILDREN:
      *r_usage = (struct rusage){};
      return 0;
    default:
      errno = EINVAL;
      return -1;
  }
}
