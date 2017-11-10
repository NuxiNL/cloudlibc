// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

uint64_t uv_timer_get_repeat(const uv_timer_t *handle) {
  return handle->__repeat;
}
