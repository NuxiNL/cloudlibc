// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

#ifndef uv_has_ref
#error "uv_has_ref is supposed to be a macro as well"
#endif

// clang-format off
int (uv_has_ref)(const uv_handle_t *handle) {
  return uv_has_ref(handle);
}
