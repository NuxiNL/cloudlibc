// Copyright (c) 2016 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "argdata_impl.h"

bool argdata_map_next(argdata_map_iterator_t *it_, const argdata_t **key,
                      const argdata_t **value) {
  struct argdata_map_iterator_impl *it =
      (struct argdata_map_iterator_impl *)it_;
  const argdata_t *ad = it->container;
  if (ad->type == AD_BUFFER) {
    const uint8_t *buf = ad->buffer + it->offset;
    size_t len = ad->length - it->offset;
    if (len == 0)
      return false;
    int error = parse_subfield(&it->key, &buf, &len);
    if (error != 0) {
      it->error = error;
      return false;
    }
    error = parse_subfield(&it->value, &buf, &len);
    if (error != 0) {
      it->error = error;
      return false;
    }
    it->offset = buf - ad->buffer;
    *key = &it->key;
    *value = &it->value;
    return true;
  } else {
    assert(ad->type == AD_MAP && "Iterator points to value of the wrong type");
    if (it->offset >= ad->map.count)
      return false;
    *key = ad->map.keys[it->offset];
    *value = ad->map.values[it->offset++];
    return true;
  }
}
