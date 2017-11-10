// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_poll_stop(uv_poll_t *poll) {
  // Discard any pending notifications.
  poll->__status = 0;
  poll->__revents = 0;

  bool disabled = false;
  if ((poll->__events & UV_READABLE) != 0) {
    __uv_reading_polls_remove(poll);
    disabled = true;
  }
  if ((poll->__events & UV_WRITABLE) != 0) {
    __uv_writing_polls_remove(poll);
    disabled = true;
  }
  poll->__events = 0;
  if (disabled)
    __uv_handle_stop((uv_handle_t *)poll);
  return 0;
}
