// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <stdbool.h>
#include <uv.h>

int uv_poll_start(uv_poll_t *handle, int events, uv_poll_cb cb) {
  uv_poll_stop(handle);

  handle->__events = events;
  handle->__cb = cb;

  uv_loop_t *loop = handle->loop;
  bool enabled = false;
  if ((events & UV_READABLE) != 0) {
    __uv_reading_polls_insert_last(&loop->__reading_polls, handle);
    enabled = true;
  }
  if ((events & UV_WRITABLE) != 0) {
    __uv_writing_polls_insert_last(&loop->__writing_polls, handle);
    enabled = true;
  }
  if (enabled)
    __uv_handle_start((uv_handle_t *)handle);
  return 0;
}
