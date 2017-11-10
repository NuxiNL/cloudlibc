// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_idle_stop(uv_idle_t *idle) {
  if (uv_is_active((uv_handle_t *)idle)) {
    __uv_handle_stop((uv_handle_t *)idle);
    __uv_active_idles_remove(idle);
  }
  return 0;
}
