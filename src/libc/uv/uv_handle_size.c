// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

size_t uv_handle_size(uv_handle_type type) {
  switch (type) {
#define ENTRY(upper, lower) \
  case UV_##upper:          \
    return sizeof(uv_##lower##_t);
    UV_HANDLE_TYPE_MAP(ENTRY)
#undef ENTRY
    default:
      return -1;
  }
}
