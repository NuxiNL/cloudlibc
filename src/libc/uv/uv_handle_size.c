// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

size_t uv_handle_size(uv_handle_type type) {
  switch (type) {
    case UV_ASYNC:
      return sizeof(uv_async_t);
    case UV_CHECK:
      return sizeof(uv_check_t);
    case UV_HANDLE:
      return sizeof(uv_handle_t);
    case UV_IDLE:
      return sizeof(uv_idle_t);
    case UV_NAMED_PIPE:
      return sizeof(uv_pipe_t);
    case UV_POLL:
      return sizeof(uv_poll_t);
    case UV_PREPARE:
      return sizeof(uv_prepare_t);
    case UV_PROCESS:
      return sizeof(uv_process_t);
    case UV_STREAM:
      return sizeof(uv_stream_t);
    case UV_TCP:
      return sizeof(uv_tcp_t);
    case UV_TIMER:
      return sizeof(uv_timer_t);
    default:
      return -1;
  }
}
