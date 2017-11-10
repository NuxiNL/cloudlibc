// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_check_start(uv_check_t *check, uv_check_cb cb) {
  check->__cb = cb;
  if (!uv_is_active((uv_handle_t *)check)) {
    __uv_active_checks_insert_last(&check->loop->__active_checks, check);
    __uv_handle_start((uv_handle_t *)check);
  }
  return 0;
}
