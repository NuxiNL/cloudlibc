// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int linkat(int fd1, const char *path1, int fd2, const char *path2, int flag) {
  // Create lookup properties.
  cloudabi_lookup_t lookup1 = (cloudabi_fd_t)fd1;
  if ((flag & AT_SYMLINK_FOLLOW) != 0)
    lookup1 |= CLOUDABI_LOOKUP_SYMLINK_FOLLOW;

  // Perform system call.
  cloudabi_errno_t error = cloudabi_sys_file_link(lookup1, path1, strlen(path1),
                                                  fd2, path2, strlen(path2));
  if (error != 0) {
    errno = errno_fixup_directory(fd1, errno_fixup_directory(fd2, error));
    return -1;
  }
  return 0;
}
