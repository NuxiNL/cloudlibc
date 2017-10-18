// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

uv_buf_t uv_buf_init(char *base, size_t len) {
  return (uv_buf_t){.base = base, .len = len};
}
