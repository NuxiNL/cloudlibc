// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

int uv_is_readable(const uv_stream_t *handle) {
  // This implementation only supports bidirectional streams.
  return handle->__fd != -1;
}
