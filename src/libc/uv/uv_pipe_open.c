// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_pipe_open(uv_pipe_t *handle, uv_file file) {
  return __uv_stream_open((uv_stream_t *)handle, file);
}
