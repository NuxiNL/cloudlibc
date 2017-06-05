// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>

#include "argdata_impl.h"

int argdata_get_int_u(const argdata_t *ad, uintmax_t *value, uintmax_t max) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_INT, &buf, &len);
      if (error != 0)
        return error;

      // Parse unsigned number.
      uintmax_t res = 0;
      if (len > 0) {
        if ((buf[0] & 0x80) != 0) {
          // Number is negative.
          return ERANGE;
        }
        if (len > sizeof(res) && (len != sizeof(res) + 1 || buf[0] != 0)) {
          // Number is too large.
          return ERANGE;
        }

        // Add digits.
        do {
          res = res << 8 | *buf++;
        } while (--len > 0);
        if (res > max)
          return ERANGE;
      }
      *value = res;
      return 0;
    }
    default:
      return EINVAL;
  }
}
