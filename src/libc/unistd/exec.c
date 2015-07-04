// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>
#include <common/syscalls.h>

#include <argdata.h>
#include <unistd.h>

int exec(int fd, const argdata_t *ad) {
  // Convert argument data to binary format.
  size_t datalen;
  size_t fdslen;
  __argdata_getspace(ad, &datalen, &fdslen);
  char data[datalen];
  int fds[fdslen];
  fdslen = __argdata_generate(ad, data, fds);

  // Invoke system call with binary data and an array of file descriptors.
  return cloudabi_sys_proc_exec(fd, data, datalen, (cloudabi_fd_t *)fds,
                                fdslen);
}
