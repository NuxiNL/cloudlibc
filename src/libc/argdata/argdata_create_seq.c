// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <stdlib.h>

argdata_t *argdata_create_seq(const argdata_t *entries, size_t count) {
  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;

  ad->type = AD_SEQ;
  ad->seq.entries = entries;
  ad->seq.count = count;
  ad->length = 1;
  for (size_t i = 0; i < count; ++i)
    ad->length += get_subfield_length(&entries[i]);
  return ad;
}
