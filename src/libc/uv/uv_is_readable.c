// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

int uv_is_readable(const uv_stream_t *handle) {
  // This implementation only supports bidirectional streams.
  return handle->__fd >= 0;
}
