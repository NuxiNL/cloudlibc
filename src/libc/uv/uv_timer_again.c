// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <stddef.h>
#include <uv.h>

int uv_timer_again(uv_timer_t *handle) {
  if (handle->__cb == NULL || handle->__repeat == 0)
    return UV_EINVAL;
  return uv_timer_start(handle, handle->__cb, handle->__repeat,
                        handle->__repeat);
}
