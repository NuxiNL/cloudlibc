// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_process_kill(uv_process_t *handle, int signum) {
  __uv_process_kill(handle);
  return 0;
}
