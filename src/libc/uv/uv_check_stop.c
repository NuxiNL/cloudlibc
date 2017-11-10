// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_check_stop(uv_check_t *check) {
  if (uv_is_active((uv_handle_t *)check)) {
    __uv_handle_stop((uv_handle_t *)check);
    __uv_active_checks_remove(check);
  }
  return 0;
}
