// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#include "argdata_impl.h"

argdata_t *argdata_create_binary(const void *buf, size_t len) {
  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;

  // Add one additional byte to the length to hold the type number.
  ad->type = AD_BINARY;
  ad->binary = buf;
  ad->length = len + 1;
  return ad;
}
