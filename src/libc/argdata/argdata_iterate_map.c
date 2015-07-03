// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

int argdata_iterate_map(const argdata_t *ad, void *thunk,
                        bool (*iterator)(const argdata_t *, const argdata_t *,
                                         void *)) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_MAP, &buf, &len);
      if (error != 0)
        return error;

      while (len > 0) {
        // Parse the key of the map entry.
        argdata_t key;
        error = parse_subfield(&key, &buf, &len);
        if (error != 0)
          return error;

        // Parse the value of the map entry.
        argdata_t value;
        error = parse_subfield(&value, &buf, &len);
        if (error != 0)
          return error;

        // Invoke iterator function with map entry.
        if (!iterator(&key, &value, thunk))
          break;
      }
      return 0;
    }
    default:
      return EINVAL;
  }
}
