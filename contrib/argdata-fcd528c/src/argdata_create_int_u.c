// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

#include "argdata_impl.h"

argdata_t *argdata_create_int_u(uintmax_t value) {
  // Allocate object with space for encoded integer value.
  static const size_t objlen = sizeof(argdata_t) + sizeof(uintmax_t) + 2;
  argdata_t *ad = malloc(objlen);
  if (ad == NULL)
    return NULL;

  // Store digits.
  uint8_t *buf = (uint8_t *)ad + objlen;
  size_t len = 0;
  while (value != 0) {
    *--buf = value;
    value >>= 8;
    ++len;
  }

  // Add additional zero byte if sign bit is set to avoid wrapping the
  // number to a negative value.
  if (len > 0 && (*buf & 0x80) != 0) {
    *--buf = 0;
    ++len;
  }

  // Add type byte.
  *--buf = ADT_INT;
  ++len;

  ad->type = AD_BUFFER;
  ad->buffer = buf;
  ad->length = len;
  return ad;
}
