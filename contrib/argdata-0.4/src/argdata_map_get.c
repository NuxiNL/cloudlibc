// Copyright (c) 2017 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdbool.h>

#include "argdata_impl.h"

bool argdata_map_get(const argdata_map_iterator_t *it_, const argdata_t **key,
                     const argdata_t **value) {
  struct argdata_map_iterator_impl *it =
      (struct argdata_map_iterator_impl *)it_;
  if (it->type == ADM_BUFFER) {
    *key = &it->buffer.key;
    *value = &it->buffer.value;
    return true;
  } else if (it->map.count > 0) {  // it->type == ADM_MAP
    *key = it->map.keys[0];
    *value = it->map.values[0];
    return true;
  } else {
    return false;
  }
}
