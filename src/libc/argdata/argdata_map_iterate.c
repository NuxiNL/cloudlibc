// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

int argdata_map_iterate(const argdata_t *ad, argdata_iterator_t *it_) {
  struct __argdata_iterator *it = (struct __argdata_iterator*)it_;
  it->container = ad;
  it->index = (size_t)-1;
  it->error = 0;
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      it->bytes_left = ad->length;
      it->error = parse_type(ADT_MAP, &buf, &it->bytes_left);
      break;
    }
    case AD_MAP:
      break;
    default:
      it->error = EINVAL;
  }
  return it->error;
}

bool argdata_map_next(argdata_iterator_t *it_,
                      const argdata_t **key,
                      const argdata_t **value) {
  struct __argdata_iterator *it = (struct __argdata_iterator*)it_;
  const argdata_t *ad = it->container;

  // Iterating already finished.
  if (ad == NULL)
    it->error = EINVAL;

  // Iterator has error.
  if (it->error != 0)
    return false;

  ++it->index;

  switch (ad->type) {
    case AD_BUFFER: {
      if (it->bytes_left == 0) {
        it->container = NULL;
        return false;
      }
      const uint8_t *buf = ad->buffer + ad->length - it->bytes_left;
      it->error = parse_subfield(&it->key, &buf, &it->bytes_left);
      if (it->error != 0)
        return false;
      it->error = parse_subfield(&it->value, &buf, &it->bytes_left);
      if (it->error != 0)
        return false;
      *key = &it->key;
      *value = &it->value;
      return true;
    }
    case AD_MAP: {
      if (it->index == ad->map.count) {
        it->container = NULL;
        return false;
      }
      *key = ad->map.keys[it->index];
      *value = ad->map.values[it->index];
      return true;
    }
    default:
      it->error = EINVAL;
      return false;
  }
}
