// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <assert.h>
#include <uv.h>

void uv_unref(uv_handle_t *handle) {
  if (uv_has_ref(handle)) {
    handle->__state &= ~UV_HANDLE_REFERENCED;
    if (uv_is_active(handle))
      --handle->loop->__active_ref_handles_reqs;
  }
}
