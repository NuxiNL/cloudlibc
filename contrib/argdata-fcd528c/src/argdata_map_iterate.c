// Copyright (c) 2016 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>

#include "argdata_impl.h"

int argdata_map_iterate(const argdata_t *ad, argdata_map_iterator_t *it_) {
  struct argdata_map_iterator_impl *it =
      (struct argdata_map_iterator_impl *)it_;
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      size_t len = ad->length;
      it->container = ad;
      it->error = parse_type(ADT_MAP, &buf, &len);
      it->offset = buf - ad->buffer;
      break;
    }
    case AD_MAP:
      it->container = ad;
      it->error = 0;
      it->offset = 0;
      break;
    default:
      it->error = EINVAL;
      break;
  }
  if (it->error != 0) {
    // If the iterator is invalid, fall back to using an empty buffer,
    // so that calls to argdata_map_next() act as if iterating an empty
    // mapping.
    it->container = &argdata_null;
    it->offset = 0;
  }
  return it->error;
}
