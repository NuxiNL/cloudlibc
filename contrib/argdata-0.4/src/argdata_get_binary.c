// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>

#include "argdata_impl.h"

int argdata_get_binary(const argdata_t *ad, const void **value,
                       size_t *valuelen) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_BINARY, &buf, &len);
      if (error != 0)
        return error;

      *value = buf;
      *valuelen = len;
      return 0;
    }
    case AD_BINARY:
      *value = ad->binary;
      *valuelen = ad->length - 1;
      return 0;
    default:
      return EINVAL;
  }
}
