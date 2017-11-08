// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

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
