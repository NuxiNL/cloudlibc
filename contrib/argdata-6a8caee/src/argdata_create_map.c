// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#include "argdata_impl.h"

argdata_t *argdata_create_map(argdata_t const *const *keys,
                              argdata_t const *const *values, size_t count) {
  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;

  ad->type = AD_MAP;
  ad->map.keys = keys;
  ad->map.values = values;
  ad->map.count = count;
  ad->length = 1;
  for (size_t i = 0; i < count; ++i) {
    ad->length += get_subfield_length(keys[i]);
    ad->length += get_subfield_length(values[i]);
  }
  return ad;
}
