// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include <errno.h>
#include <uv.h>

int uv_inet_pton(int af, const char *src, void *dst) {
  int ret = inet_pton(af, src, dst);
  if (ret > 0)
    return 0;
  if (ret < 0)
    return -errno;
  return UV_EINVAL;
}
