// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/stat.h>

#include <errno.h>

#include "stat_impl.h"

int fstat(int fildes, struct stat *buf) {
  cloudabi_filestat_t internal_stat;
  cloudabi_errno_t error = cloudabi_sys_file_stat_fget(fildes, &internal_stat);
  if (error != 0) {
    errno = error;
    return -1;
  }
  to_public_stat(&internal_stat, buf);
  return 0;
}
