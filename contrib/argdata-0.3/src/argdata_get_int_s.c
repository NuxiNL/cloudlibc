// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>

#include "argdata_impl.h"

int argdata_get_int_s(const argdata_t *ad, intmax_t *value, intmax_t min,
                      intmax_t max) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_INT, &buf, &len);
      if (error != 0)
        return error;

      // Parse signed number.
      intmax_t res = 0;
      unsigned int shift = sizeof(res) * CHAR_BIT - 8;
      if (len > 0) {
        if (len > sizeof(res)) {
          // Number is too large.
          return ERANGE;
        }

        // Add digits.
        for (;;) {
          res |= (uintmax_t)*buf++ << shift;
          if (--len == 0)
            break;
          shift -= 8;
        }
        res >>= shift;
        if (res < min || res > max)
          return ERANGE;
      }
      *value = res;
      return 0;
    }
    default:
      return EINVAL;
  }
}
