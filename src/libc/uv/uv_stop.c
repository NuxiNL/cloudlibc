// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdbool.h>
#include <uv.h>

void uv_stop(uv_loop_t *loop) {
  loop->__stop = true;
}
