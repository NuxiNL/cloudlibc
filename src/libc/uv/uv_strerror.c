// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

const char *uv_strerror(int err) {
  switch (err) {
#define ENTRY(name, description) \
  case UV_##name:                \
    return description;
    UV_ERRNO_MAP(ENTRY)
#undef ENTRY
    default:
      return __uv_strerror_unknown(err);
  }
}
