// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

uv_os_fd_t uv_get_osfhandle(int fd) {
  return fd;
}
