// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_idle_start(uv_idle_t *idle, uv_idle_cb cb) {
  idle->__cb = cb;
  if (!uv_is_active((uv_handle_t *)idle)) {
    __uv_active_idles_insert_last(&idle->loop->__active_idles, idle);
    __uv_handle_start((uv_handle_t *)idle);
  }
  return 0;
}
