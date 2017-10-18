// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_poll_init(uv_loop_t *loop, uv_poll_t *handle, int fd) {
  __uv_handle_init(loop, (uv_handle_t *)handle, UV_POLL);

  // TODO(ed): Make the file descriptor non-blocking.

  handle->__fd = -1;
  return 0;
}

__strong_reference(uv_poll_init, uv_poll_init_socket);
