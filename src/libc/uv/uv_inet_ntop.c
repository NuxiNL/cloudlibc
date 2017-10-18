// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <errno.h>
#include <stddef.h>
#include <uv.h>

int uv_inet_ntop(int af, const void *src, char *dst, size_t size) {
  return inet_ntop(af, src, dst, size) == NULL ? -errno : 0;
}
