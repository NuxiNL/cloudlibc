// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_ARGDATA_H
#define COMMON_ARGDATA_H

#include <argdata.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
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

enum {
  ADT_BINARY = 1,    // A sequence of zero or more octets.
  ADT_BOOL = 2,      // Mathematical Booleans.
  ADT_FD = 3,        // Extension to YAML: file descriptor.
  ADT_FLOAT = 4,     // Floating-point approximation to real numbers.
  ADT_INT = 5,       // Mathematical integers.
  ADT_MAP = 6,       // Unordered set of key: value pairs without duplicates.
  ADT_SEQ = 7,       // Sequence of arbitrary values.
  ADT_STR = 8,       // A sequence of zero or more Unicode characters.
  ADT_TIMESTAMP = 9  // A point in time.
};

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
  // Parse the field length. The length is stored in big endian form,
  // seven bits per byte. The top bit is used to indicate whether the
  // last byte of the field length has been reached.
  size_t reclen = 0;
  for (;;) {
    // Fetch digit.
    if ((*len)-- == 0)
      return EINVAL;
    uint8_t c = *(*buf)++;

    // Make space to fit the digit.
    if (reclen >> (sizeof(reclen) * CHAR_BIT - 7) != 0)
      return EINVAL;
    reclen <<= 7;

    if ((c & 0x80) != 0) {
      // Last byte.
      reclen |= c & 0x7f;
      break;
    } else {
      // More bytes to come.
      reclen |= c;
    }
  }

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
