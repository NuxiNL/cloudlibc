// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_ARGDATA_H
#define COMMON_ARGDATA_H

#include <argdata.h>
#include <errno.h>
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

enum { ADT_BOOL = 1, ADT_FD = 2, ADT_MAP = 3, ADT_SEQ = 4, ADT_STR = 5 };

static inline void argdata_init_binary(argdata_t *ad, const void *buf,
                                       size_t len) {
  ad->type = AD_BUFFER;
  ad->buffer.buf = buf;
  ad->buffer.len = len;
}

// Parses a 32-bit unsigned integer in the input stream.
static inline int parse_uint32(uint32_t *val, const uint8_t **buf,
                               size_t *len) {
  if (*len < sizeof(uint32_t))
    return EINVAL;
  *val = (uint32_t)(*buf)[0] << 24 | (uint32_t)(*buf)[1] << 16 |
         (uint32_t)(*buf)[2] << 8 | (uint32_t)(*buf)[3];
  *buf += sizeof(uint32_t);
  *len -= sizeof(uint32_t);
  return 0;
}

// Parses a field embedded in the input stream.
static inline int parse_subfield(argdata_t *ad, const uint8_t **buf,
                                 size_t *len) {
  uint32_t reclen;
  int error = parse_uint32(&reclen, buf, len);
  if (error != 0)
    return error;

  if (reclen > *len)
    return EINVAL;
  argdata_init_binary(ad, *buf, reclen);
  *buf += reclen;
  *len -= reclen;
  return 0;
}

// Parses a type byte in the input stream.
static inline int parse_type(uint8_t type, const uint8_t **buf, size_t *len) {
  if (*len < 1 || **buf != type)
    return EINVAL;
  ++*buf;
  --*len;
  return 0;
}

#endif
