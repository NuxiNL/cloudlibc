// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

size_t uv_loop_size(void) {
  return sizeof(uv_loop_t);
}
