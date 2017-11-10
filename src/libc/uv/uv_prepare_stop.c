// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_prepare_stop(uv_prepare_t *prepare) {
  if (uv_is_active((uv_handle_t *)prepare)) {
    __uv_handle_stop((uv_handle_t *)prepare);
    __uv_active_prepares_remove(prepare);
  }
  return 0;
}
