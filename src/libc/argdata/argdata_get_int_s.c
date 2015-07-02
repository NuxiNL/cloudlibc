// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdint.h>

int __argdata_get_int_s(const argdata_t *ad, intmax_t *value, intmax_t min,
                        intmax_t max) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buf;
      size_t len = ad->buffer.len;
      int error = parse_type(ADT_INT, &buf, &len);
      if (error != 0)
        return error;

      // TODO(ed): Implement.
      return ENOSYS;
    }
    default:
      return EINVAL;
  }
}
