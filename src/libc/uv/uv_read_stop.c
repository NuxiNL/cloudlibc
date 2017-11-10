// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <stddef.h>
#include <uv.h>

int uv_read_stop(uv_stream_t *stream) {
  if (stream->__read_cb != NULL) {
    stream->__read_cb = NULL;
    __uv_stream_stop_reading(stream);
  }
  return 0;
}
