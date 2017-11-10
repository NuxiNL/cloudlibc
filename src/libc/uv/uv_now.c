// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

uint64_t uv_now(const uv_loop_t *loop) {
  return loop->__now;
}
