// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdlib.h>

#include "argdata_impl.h"

argdata_t *argdata_create_str(const char *buf, size_t len) {
  // Validate the string for encoding errors.
  int error = validate_string(buf, len);
  if (error != 0) {
    errno = error;
    return NULL;
  }

  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;

  // Add two additional byte to the length to hold the type number and
  // the terminating null byte of the string.
  ad->type = AD_STR;
  ad->str = buf;
  ad->length = len + 2;
  return ad;
}
