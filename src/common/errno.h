// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_ERRNO_H
#define COMMON_ERRNO_H

#include <common/syscalls.h>

// Translates ENOTCAPABLE to ENOTDIR if not a directory.
static inline cloudabi_errno_t errno_fixup_directory(cloudabi_fd_t fd,
                                                     cloudabi_errno_t error) {
  if (error == CLOUDABI_ENOTCAPABLE) {
    cloudabi_fdstat_t fds;
    if (cloudabi_sys_fd_stat_get(fd, &fds) == 0 &&
        fds.fs_filetype != CLOUDABI_FILETYPE_DIRECTORY)
      return CLOUDABI_ENOTDIR;
  }
  return error;
}

// Translates ENOTCAPABLE to EINVAL if not a process.
static inline cloudabi_errno_t errno_fixup_process(cloudabi_fd_t fd,
                                                   cloudabi_errno_t error) {
  if (error == CLOUDABI_ENOTCAPABLE) {
    cloudabi_fdstat_t fds;
    if (cloudabi_sys_fd_stat_get(fd, &fds) == 0 &&
        fds.fs_filetype != CLOUDABI_FILETYPE_PROCESS)
      return CLOUDABI_EINVAL;
  }
  return error;
}

// Translates ENOTCAPABLE to ENOTSOCK if not a socket.
static inline cloudabi_errno_t errno_fixup_socket(cloudabi_fd_t fd,
                                                  cloudabi_errno_t error) {
  if (error == CLOUDABI_ENOTCAPABLE) {
    cloudabi_fdstat_t fds;
    if (cloudabi_sys_fd_stat_get(fd, &fds) == 0 &&
        (fds.fs_filetype & 0xf0) != 0x80)
      return CLOUDABI_ENOTSOCK;
  }
  return error;
}

#endif
