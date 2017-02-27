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
    case AD_BUFFER:
      it->buf = ad->buffer;
      it->len = ad->length;
      it->error = parse_type(ADT_MAP, &it->buf, &it->len);
      it->values = NULL;
      break;
    case AD_MAP:
      it->keys = ad->map.keys;
      it->values = ad->map.values;
      it->len = ad->map.count;
      it->error = 0;
      break;
    default:
      it->error = EINVAL;
      break;
  }
  if (it->error != 0) {
    // If the iterator is invalid, set len to zero so that calls to
    // argdata_map_next() act as if iterating an empty mapping.
    it->len = 0;
  }
  return it->error;
}
