// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

uv_handle_type uv_pipe_pending_type(uv_pipe_t *handle) {
  return __uv_pending_fds_empty(&handle->__pending_fds)
             ? UV_UNKNOWN_HANDLE
             : uv_guess_handle(__uv_pending_fds_first(&handle->__pending_fds));
}
