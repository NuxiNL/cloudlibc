// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

#include "argdata_impl.h"

argdata_t *argdata_create_int_s(intmax_t value) {
  // Treat non-negative numbers as unsigned.
  if (value >= 0)
    return argdata_create_int_u(value);

  // Allocate object with space for encoded integer value.
  static const size_t objlen = sizeof(argdata_t) + sizeof(uintmax_t) + 1;
  argdata_t *ad = malloc(objlen);
  if (ad == NULL)
    return NULL;

  // Store digits.
  uint8_t *buf = (uint8_t *)ad + objlen;
  size_t len = 0;
  do {
    *--buf = value;
    value >>= 8;
    ++len;
  } while (value != -1 || (*buf & 0x80) == 0);

  // Add type byte.
  *--buf = ADT_INT;
  ++len;

  ad->type = AD_BUFFER;
  ad->buffer = buf;
  ad->length = len;
  return ad;
}
