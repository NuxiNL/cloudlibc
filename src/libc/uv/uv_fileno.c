// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

int uv_fileno(const uv_handle_t *handle, uv_os_fd_t *fd) {
  switch (handle->type) {
    case UV_NAMED_PIPE:
    case UV_TCP:
    case UV_TTY: {
      uv_stream_t *stream = (uv_stream_t *)handle;
      if (stream->__fd < 0)
        return UV_EBADF;
      *fd = stream->__fd;
      return 0;
    }
    case UV_POLL: {
      if (uv_is_closing(handle))
        return UV_EBADF;
      uv_poll_t *poll = (uv_poll_t *)handle;
      *fd = poll->__fd;
      return 0;
    }
    default:
      return UV_EINVAL;
  }
}
