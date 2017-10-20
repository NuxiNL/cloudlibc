// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_poll_stop(uv_poll_t *poll) {
  // Discard any pending notifications.
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
