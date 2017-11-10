// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <uv.h>

void uv_stop(uv_loop_t *loop) {
  loop->__stop = true;
}
