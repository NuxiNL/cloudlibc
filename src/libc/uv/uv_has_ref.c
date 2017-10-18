// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

#ifndef uv_has_ref
#error "uv_has_ref is supposed to be a macro as well"
#endif

// clang-format off
int (uv_has_ref)(const uv_handle_t *handle) {
  return uv_has_ref(handle);
}
