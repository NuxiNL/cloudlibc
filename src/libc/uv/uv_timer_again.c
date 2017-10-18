// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <uv.h>

#include "uv_impl.h"

int uv_timer_again(uv_timer_t *handle) {
  if (handle->__cb == NULL || handle->__repeat == 0)
    return UV_EINVAL;
  return uv_timer_start(handle, handle->__cb, handle->__repeat,
                        handle->__repeat);
}
