// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

int uv_process_kill(uv_process_t *handle, int signum) {
  __uv_process_kill(handle);
  return 0;
}
