// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_pipe_pending_count(uv_pipe_t *handle) {
  return __uv_pending_fds_count(&handle->__pending_fds);
}
