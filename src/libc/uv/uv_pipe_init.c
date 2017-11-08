// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

int uv_pipe_init(uv_loop_t *loop, uv_pipe_t *handle, int ipc) {
  __uv_stream_init(loop, (uv_stream_t *)handle, UV_NAMED_PIPE);
  handle->ipc = ipc;
  return 0;
}
