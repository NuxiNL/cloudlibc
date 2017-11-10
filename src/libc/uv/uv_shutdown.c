// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_shutdown(uv_shutdown_t *req, uv_stream_t *handle, uv_shutdown_cb cb) {
  if (handle->__fd < 0)
    return UV_EBADF;

  // Initialize the shutdown request.
  req->handle = handle;
  req->__cb = cb;
  __uv_req_init((uv_req_t *)req, UV_SHUTDOWN);

  // Enqueue the shutdown request.
  __uv_stream_start_writing(handle);
  __uv_shutdowns_insert_last(&handle->__shutdown_queue, req);
  return 0;
}
