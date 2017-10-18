// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_prepare_start(uv_prepare_t *prepare, uv_prepare_cb cb) {
  prepare->__cb = cb;
  if (!uv_is_active((uv_handle_t *)prepare)) {
    __uv_active_prepares_insert_last(&prepare->loop->__active_prepares,
                                     prepare);
    __uv_handle_start((uv_handle_t *)prepare);
  }
  return 0;
}
