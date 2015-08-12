// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "stat_impl.h"

int utimensat(int fd, const char *path, const struct timespec times[2],
              int flag) {
  // Convert timestamps and extract NOW/OMIT flags.
  cloudabi_filestat_t fs;
  cloudabi_fsflags_t flags;
  if (!utimens_get_timestamps(times, &fs, &flags)) {
    errno = EINVAL;
    return -1;
  }

  // Create lookup properties.
  cloudabi_lookup_t lookup = (cloudabi_fd_t)fd;
  if ((flag & AT_SYMLINK_NOFOLLOW) == 0)
    lookup |= CLOUDABI_LOOKUP_SYMLINK_FOLLOW;

  // Perform system call.
  cloudabi_errno_t error =
      cloudabi_sys_file_stat_put(lookup, path, strlen(path), &fs, flags);
  if (error != 0) {
    errno = errno_fixup_directory(fd, error);
    return -1;
  }
  return 0;
}
