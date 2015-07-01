// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>

int argdata_get_string(const argdata_t *ad, const char **value, size_t *len) {
  switch (ad->type) {
    case AD_BUFFER:
      if (ad->buffer.len < 2 || ad->buffer.buf[0] != ADT_STRING ||
          ad->buffer.buf[ad->buffer.len - 1] != '\0')
        return EINVAL;
      *value = (const char *)ad->buffer.buf + 1;
      *len = ad->buffer.len - 2;
      return 0;
    default:
      return EINVAL;
  }
}
