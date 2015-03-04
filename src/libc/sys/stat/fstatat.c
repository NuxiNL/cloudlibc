// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "stat_impl.h"

int fstatat(int fd, const char *restrict path, struct stat *restrict buf,
            int flag) {
  // Create lookup properties.
  cloudabi_lookup_t lookup = (cloudabi_fd_t)fd;
  if ((flag & AT_SYMLINK_NOFOLLOW) == 0)
    lookup |= CLOUDABI_LOOKUP_SYMLINK_FOLLOW;

  // Perform system call.
  cloudabi_filestat_t internal_stat;
  cloudabi_errno_t error =
      cloudabi_sys_file_stat_get(lookup, path, strlen(path), &internal_stat);
  if (error != 0) {
    errno = error;
    return -1;
  }
  to_public_stat(&internal_stat, buf);
  return 0;
}
