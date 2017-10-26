// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

size_t uv_req_size(uv_req_type type) {
  switch (type) {
#define ENTRY(upper, lower) \
  case UV_##upper:          \
    return sizeof(uv_##lower##_t);
    UV_REQ_TYPE_MAP(ENTRY)
#undef ENTRY
    default:
      return -1;
  }
}
