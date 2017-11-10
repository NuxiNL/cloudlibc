// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <stdlib.h>
#include <string.h>
#include <uv.h>

int uv_write2(uv_write_t *req, uv_stream_t *handle, const uv_buf_t *bufs,
              unsigned int nbufs, uv_stream_t *send_handle, uv_write_cb cb) {
  // Sanity checks on the write request.
  if (handle->__fd < 0 || (send_handle != NULL && send_handle->__fd < 0))
    return UV_EBADF;
  if (send_handle != NULL && !__uv_stream_has_ipc(handle))
    return UV_EINVAL;

  // Never create write requests with zero buffers, as we assume that
  // requests that are in flight have at least one buffer.
  if (nbufs == 0) {
    static const uv_buf_t empty_buf = {};
    bufs = &empty_buf;
    nbufs = 1;
  }

  // Initialize the write request.
  req->handle = handle;
  req->send_handle = send_handle;
  req->__bufs = reallocarray(NULL, sizeof(uv_buf_t), nbufs);
  if (req->__bufs == NULL)
    return UV_ENOMEM;
  memcpy(req->__bufs, bufs, sizeof(uv_buf_t) * nbufs);
  req->__nbufs_done = 0;
  req->__nbufs_total = nbufs;
  req->__cb = cb;
  __uv_req_init((uv_req_t *)req, UV_WRITE);

  // Enqueue the write request.
  __uv_stream_start_writing(handle);
  __uv_writes_insert_last(&handle->__write_queue, req);
  for (unsigned int i = 0; i < req->__nbufs_total; ++i)
    handle->write_queue_size += req->__bufs[i].len;
  return 0;
}
