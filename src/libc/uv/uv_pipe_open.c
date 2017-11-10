// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_pipe_open(uv_pipe_t *handle, uv_file file) {
  return __uv_stream_open((uv_stream_t *)handle, file);
}
