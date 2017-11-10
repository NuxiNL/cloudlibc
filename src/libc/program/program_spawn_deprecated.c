// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata.h>
#include <cloudabi_syscalls.h>
#include <errno.h>
#include <program.h>

int program_spawn_deprecated(int fd, const argdata_t *ad) {
  // Fork the current process.
  cloudabi_fd_t ret;
  cloudabi_tid_t tid;
  cloudabi_errno_t error = cloudabi_sys_proc_fork(&ret, &tid);
  if (error != 0) {
    errno = error;
    return -1;
  }

  if (ret == CLOUDABI_PROCESS_CHILD) {
    // Child process. Start the executable.
    program_exec(fd, ad);
    cloudabi_sys_proc_exit(127);
  } else {
    // Parent process. Return the file descriptor.
    return ret;
  }
}
