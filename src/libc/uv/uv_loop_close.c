// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <uv.h>

#include "uv_impl.h"

int uv_loop_close(uv_loop_t *loop) {
  // TODO(ed): Free memory and return UV_EBUSY when needed.
  __uv_active_timers_destroy(&loop->__active_timers);

  free(loop->__subscriptions_buffer);
  free(loop->__events_buffer);
  return 0;
}
