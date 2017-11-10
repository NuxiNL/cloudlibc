// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

uv_buf_t uv_buf_init(char *base, size_t len) {
  return (uv_buf_t){.base = base, .len = len};
}
