// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/procdesc.h>

#include <errno.h>

int pdexec(int fd, const struct iovec *arg, size_t argcnt, const int *fds,
           size_t fdscnt) {
  // Fork the current process. There is no need to deal with
  // pthread_atfork handlers, as we are going to execute a new program
  // directly after forking.
  cloudabi_fd_t ret;
  cloudabi_tid_t tid;
  cloudabi_errno_t error = cloudabi_sys_proc_fork(&ret, &tid);
  if (error != 0) {
    errno = error;
    return -1;
  }

  if (ret == CLOUDABI_PROCESS_CHILD) {
    // Child process. Start the executable.
    cloudabi_sys_proc_exec(fd, (const cloudabi_ciovec_t *)arg, argcnt,
                           (const cloudabi_fd_t *)fds, fdscnt);
    cloudabi_sys_proc_exit(127);
  } else {
    // Parent process. Return the file descriptor.
    return ret;
  }
}
