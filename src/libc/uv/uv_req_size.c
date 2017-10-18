// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

size_t uv_req_size(uv_req_type type) {
  switch (type) {
    case UV_FS:
      return sizeof(uv_fs_t);
    case UV_GETADDRINFO:
      return sizeof(uv_getaddrinfo_t);
    case UV_GETNAMEINFO:
      return sizeof(uv_getnameinfo_t);
    case UV_REQ:
      return sizeof(uv_req_t);
    case UV_SHUTDOWN:
      return sizeof(uv_shutdown_t);
    case UV_WORK:
      return sizeof(uv_work_t);
    case UV_WRITE:
      return sizeof(uv_write_t);
    default:
      return -1;
  }
}
