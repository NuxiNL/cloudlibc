// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <stdlib.h>

argdata_t *argdata_create_buffer(const void *buf, size_t len) {
  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;
  argdata_init_buffer(ad, buf, len);
  return ad;
}
