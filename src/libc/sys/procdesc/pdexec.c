// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>
#include <common/syscalls.h>

#include <sys/procdesc.h>

#include <argdata.h>
#include <errno.h>
#include <stdlib.h>

int pdexec(int fd, const argdata_t *ad) {
  // Convert argument data to binary format.
  size_t datalen = ad->length;
  char data[datalen];
  int *fds;
  size_t fdslen;
  {
    int error = __argdata_generate(ad, data, &fds, &fdslen);
    if (error != 0) {
      errno = error;
      return -1;
    }
  }

  // Fork the current process. There is no need to deal with
  // pthread_atfork handlers, as we are going to execute a new program
  // directly after forking.
  cloudabi_fd_t ret;
  cloudabi_tid_t tid;
  cloudabi_errno_t error = cloudabi_sys_proc_fork(&ret, &tid);
  if (error != 0) {
    free(fds);
    errno = error;
    return -1;
  }

  if (ret == CLOUDABI_PROCESS_CHILD) {
    // Child process. Start the executable.
    // TODO(ed): Convert argument data to values passed to exec().
    cloudabi_sys_proc_exec(fd, data, datalen, (const cloudabi_fd_t *)fds,
                           fdslen);
    cloudabi_sys_proc_exit(127);
  } else {
    // Parent process. Return the file descriptor.
    free(fds);
    return ret;
  }
}
