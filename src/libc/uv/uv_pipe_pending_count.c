// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_pipe_pending_count(uv_pipe_t *handle) {
  return __uv_pending_fds_count(&handle->__pending_fds);
}
