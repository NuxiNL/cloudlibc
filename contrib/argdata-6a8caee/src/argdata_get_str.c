// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>

#include "argdata_impl.h"

int argdata_get_str(const argdata_t *ad, const char **value, size_t *valuelen) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_STR, &buf, &len);
      if (error != 0)
        return error;

      // Validate the string for encoding errors.
      if (len < 1 || buf[len - 1] != '\0')
        return EINVAL;
      error = validate_string((const char *)buf, len - 1);
      if (error != 0)
        return error;
      *value = (const char *)buf;
      *valuelen = len - 1;
      return 0;
    }
    case AD_STR:
      *value = ad->str;
      *valuelen = ad->length - 2;
      return 0;
    default:
      return EINVAL;
  }
}
