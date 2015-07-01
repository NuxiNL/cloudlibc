// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdint.h>

int argdata_iterate_seq(const argdata_t *ad, void *thunk,
                        _Bool (*iterator)(const argdata_t *, void *)) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buf;
      size_t len = ad->buffer.len;
      int error = parse_type(ADT_SEQ, &buf, &len);
      if (error != 0)
        return error;

      while (len > 0) {
        // Parse the value of the sequence entry.
        argdata_t value;
        error = parse_subfield(&value, &buf, &len);
        if (error != 0)
          return error;

        // Invoke iterator function with sequence entry.
        if (!iterator(&value, thunk))
          break;
      }
      return 0;
    }
    default:
      return EINVAL;
  }
}
