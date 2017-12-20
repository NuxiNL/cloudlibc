// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <net/if.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <uv.h>

static_assert(UV_IF_NAMESIZE == IF_NAMESIZE + 1, "Size mismatch");

int uv_if_indextoname(unsigned int ifindex, char *buffer, size_t *size) {
  char name[IF_NAMESIZE];
  if (if_indextoname(ifindex, name) == NULL)
    return -errno;

  size_t len = strnlen(name, sizeof(name));
  if (*size <= len) {
    *size = len + 1;
    return UV_ENOBUFS;
  }

  memcpy(buffer, name, len);
  buffer[len] = '\0';
  *size = len;
  return 0;
}

__strong_reference(uv_if_indextoname, uv_if_indextoiid);
