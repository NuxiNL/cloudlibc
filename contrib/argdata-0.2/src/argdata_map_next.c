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
  if (it->len == 0)
    return false;
  if (it->values == NULL) {
    int error = parse_subfield(&it->key, &it->buf, &it->len);
    if (error != 0) {
      it->error = error;
      return false;
    }
    error = parse_subfield(&it->value, &it->buf, &it->len);
    if (error != 0) {
      it->error = error;
      return false;
    }
    *key = &it->key;
    *value = &it->value;
    return true;
  } else {
    *key = *it->keys++;
    *value = *it->values++;
    --it->len;
    return true;
  }
}
