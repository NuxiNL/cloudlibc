// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>

#include "argdata_impl.h"

int argdata_get_fd(const argdata_t *ad, int *value) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_FD, &buf, &len);
      if (error != 0)
        return error;

      // Extract file descriptor number.
      return parse_fd(value, &buf, &len);
    }
    default:
      return EINVAL;
  }
}
