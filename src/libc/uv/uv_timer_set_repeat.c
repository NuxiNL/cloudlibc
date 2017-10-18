// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

void uv_timer_set_repeat(uv_timer_t *handle, uint64_t repeat) {
  handle->__repeat = repeat;
}
