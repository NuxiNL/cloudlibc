// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_syscalls.h>
#include <uv.h>

uv_handle_type uv_guess_handle(uv_file file) {
  cloudabi_fdstat_t fds;
  if (cloudabi_sys_fd_stat_get(file, &fds) != 0)
    return UV_UNKNOWN_HANDLE;

  switch (fds.fs_filetype) {
    case CLOUDABI_FILETYPE_BLOCK_DEVICE:
    case CLOUDABI_FILETYPE_REGULAR_FILE:
      return UV_FILE;
    case CLOUDABI_FILETYPE_CHARACTER_DEVICE:
    case CLOUDABI_FILETYPE_SOCKET_STREAM:
      return UV_NAMED_PIPE;
    case CLOUDABI_FILETYPE_SOCKET_DGRAM:
      return UV_UDP;
    default:
      return UV_UNKNOWN_HANDLE;
  }
}
