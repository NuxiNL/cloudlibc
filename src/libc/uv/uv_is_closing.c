// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

#ifndef uv_is_closing
#error "uv_is_closing is supposed to be a macro as well"
#endif

// clang-format off
int (uv_is_closing)(const uv_handle_t *handle) {
  return uv_is_closing(handle);
}
