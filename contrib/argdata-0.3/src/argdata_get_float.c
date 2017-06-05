// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <float.h>

#include "argdata_impl.h"

int argdata_get_float(const argdata_t *ad, double *value) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_FLOAT, &buf, &len);
      if (error != 0)
        return error;

      // Extract floating point value.
      if (len != sizeof(uint64_t))
        return EINVAL;
      uint64_t bits = 0;
      for (size_t i = 0; i < sizeof(uint64_t); ++i)
        bits = bits << 8 | buf[i];
#if DBL_MANT_DIG == 53
      union {
        uint64_t i;
        double f;
      } v = {.i = bits};
      *value = v.f;
#else
#error "Unknown format for double"
#endif
      return 0;
    }
    default:
      return EINVAL;
  }
}
