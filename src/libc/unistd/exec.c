// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <unistd.h>

int exec(int fd, const struct iovec *arg, size_t argcnt, const int *fds,
         size_t fdscnt) {
  return cloudabi_sys_proc_exec(fd, (const cloudabi_ciovec_t *)arg, argcnt,
                                (const cloudabi_fd_t *)fds, fdscnt);
}
