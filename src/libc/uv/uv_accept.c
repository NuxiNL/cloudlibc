// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_accept(uv_stream_t *server, uv_stream_t *client) {
  // Check whether stream has any pending incoming file descriptors.
  if (__uv_pending_fds_empty(&server->__pending_fds))
    return UV_EAGAIN;

  // Move the pending file descriptor into the target stream object.
  int error =
      __uv_stream_open(client, __uv_pending_fds_first(&server->__pending_fds));
  if (error != 0)
    return error;
  __uv_pending_fds_remove_first(&server->__pending_fds);
  return 0;
}
