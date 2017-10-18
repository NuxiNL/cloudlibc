// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_prepare_stop(uv_prepare_t *prepare) {
  if (uv_is_active((uv_handle_t *)prepare)) {
    __uv_handle_stop((uv_handle_t *)prepare);
    __uv_active_prepares_remove(prepare);
  }
  return 0;
}
