// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <stdlib.h>

argdata_t *argdata_create_str(const char *buf, size_t len) {
  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;

  // Add two additional byte to the length to hold the type number and
  // the terminating null byte of the string.
  // TODO(edje): Validate UTF-8.
  ad->type = AD_STR;
  ad->str = buf;
  ad->length = len + 2;
  return ad;
}
