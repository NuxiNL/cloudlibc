// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

void uv_ref(uv_handle_t *handle) {
  if (!uv_has_ref(handle)) {
    handle->__state |= UV_HANDLE_REFERENCED;
    if (uv_is_active(handle))
      ++handle->loop->__active_ref_handles_reqs;
  }
}
