// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_ARGDATA_H
#define COMMON_ARGDATA_H

#include <argdata.h>
#include <stddef.h>
#include <stdint.h>

struct __argdata {
  enum { AD_BUFFER } type;
  union {
    struct {
      const uint8_t *buf;
      size_t len;
    } buffer;
  };
};

enum { ADT_BOOL = 1, ADT_FD = 2, ADT_STRING = 3 };

static inline void argdata_init_binary(argdata_t *ad, const void *buf,
                                       size_t len) {
  ad->type = AD_BUFFER;
  ad->buffer.buf = buf;
  ad->buffer.len = len;
}

#endif
