// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <cloudabi_syscalls.h>
#include <uv.h>

int uv_stream_set_blocking(uv_stream_t *handle, int blocking) {
  if (handle->__fd < 0)
    return UV_EBADF;

  // Fetch existing file descriptor flags.
  cloudabi_fdstat_t fds;
  cloudabi_errno_t error = cloudabi_sys_fd_stat_get(handle->__fd, &fds);
  if (error != 0)
    return -error;

  // Toggle the blocking bit.
  if (blocking)
    fds.fs_flags &= ~CLOUDABI_FDFLAG_NONBLOCK;
  else
    fds.fs_flags |= CLOUDABI_FDFLAG_NONBLOCK;

  // Set new flags.
  return -cloudabi_sys_fd_stat_put(handle->__fd, &fds, CLOUDABI_FDSTAT_FLAGS);
}
