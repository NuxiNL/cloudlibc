// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <uv.h>

#include "uv_impl.h"

int uv_read_stop(uv_stream_t *stream) {
  if (stream->__read_cb != NULL) {
    stream->__read_cb = NULL;
    __uv_stream_stop_reading(stream);
  }
  return 0;
}
