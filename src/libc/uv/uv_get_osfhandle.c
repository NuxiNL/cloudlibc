// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

uv_os_fd_t uv_get_osfhandle(int fd) {
  return fd;
}
