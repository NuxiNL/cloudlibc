// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

int uv_tty_init(uv_loop_t *loop, uv_tty_t *handle, uv_file fd, int readable) {
  // TODO(ed): Respect 'readable' flag.
  __uv_stream_init(loop, (uv_stream_t *)handle, UV_TTY);
  int error = __uv_stream_open((uv_stream_t *)handle, fd);
  if (error != 0)
    __uv_handles_remove((uv_handle_t *)handle);
  return error;
}
