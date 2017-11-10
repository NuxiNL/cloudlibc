// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

void uv_timer_set_repeat(uv_timer_t *handle, uint64_t repeat) {
  handle->__repeat = repeat;
}
