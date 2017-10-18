// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_check_stop(uv_check_t *check) {
  if (uv_is_active((uv_handle_t *)check)) {
    __uv_handle_stop((uv_handle_t *)check);
    __uv_active_checks_remove(check);
  }
  return 0;
}
