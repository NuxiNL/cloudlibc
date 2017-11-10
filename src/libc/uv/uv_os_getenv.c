// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>
#include <uv.h>

int uv_os_getenv(const char *name, char *buffer, size_t *size) {
  const char *value = getenv(name);
  if (value == NULL)
    return UV_ENOENT;
  size_t len = strlen(value);
  if (len >= *size) {
    *size = len + 1;
    return UV_ENOBUFS;
  }
  memcpy(buffer, value, len + 1);
  *size = len;
  return 0;
}
