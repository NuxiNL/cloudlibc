// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <argdata.h>
#include <program.h>

int program_exec(int fd, const argdata_t *ad) {
  // Convert argument data to binary format.
  size_t datalen;
  size_t fdslen;
  argdata_get_buffer_length(ad, &datalen, &fdslen);
  char data[datalen];
  int fds[fdslen];
  fdslen = argdata_get_buffer(ad, data, fds);

  // Invoke system call with binary data and an array of file descriptors.
  return cloudabi_sys_proc_exec(fd, data, datalen, (cloudabi_fd_t *)fds,
                                fdslen);
}
