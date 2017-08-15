// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdbool.h>

#include "argdata_impl.h"

int argdata_get_bool(const argdata_t *ad, bool *value) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_BOOL, &buf, &len);
      if (error != 0)
        return error;

      // Extract boolean value.
      if (len == 0) {
        // False.
        *value = false;
        return 0;
      } else if (len == 1 && buf[0] == 1) {
        // True.
        *value = true;
        return 0;
      }
      return EINVAL;
    }
    default:
      return EINVAL;
  }
}
