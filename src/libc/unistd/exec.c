// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>
#include <common/syscalls.h>

#include <argdata.h>
#include <stdlib.h>
#include <unistd.h>

int exec(int fd, const argdata_t *ad) {
  // Convert argument data to binary format.
  size_t datalen = ad->length;
  void *data[datalen];
  int *fds;
  size_t fdslen;
  {
    int error = __argdata_generate(ad, data, &fds, &fdslen);
    if (error != 0)
      return error;
  }

  // Invoke system call with binary data and an array of file descriptors.
  cloudabi_errno_t error =
      cloudabi_sys_proc_exec(fd, data, datalen, (cloudabi_fd_t *)fds, fdslen);
  free(fds);
  return error;
}
