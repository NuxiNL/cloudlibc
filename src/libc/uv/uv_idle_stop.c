// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_idle_stop(uv_idle_t *idle) {
  if (uv_is_active((uv_handle_t *)idle)) {
    __uv_handle_stop((uv_handle_t *)idle);
    __uv_active_idles_remove(idle);
  }
  return 0;
}
