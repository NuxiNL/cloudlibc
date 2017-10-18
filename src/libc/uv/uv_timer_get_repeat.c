// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

uint64_t uv_timer_get_repeat(const uv_timer_t *handle) {
  return handle->__repeat;
}
