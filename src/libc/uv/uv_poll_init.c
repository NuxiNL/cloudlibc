// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_poll_init(uv_loop_t *loop, uv_poll_t *handle, int fd) {
  __uv_handle_init(loop, (uv_handle_t *)handle, UV_POLL);

  handle->__fd = fd;
  handle->__events = 0;
  return 0;
}

__strong_reference(uv_poll_init, uv_poll_init_socket);
