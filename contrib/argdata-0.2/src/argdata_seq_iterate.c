// Copyright (c) 2016 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>

#include "argdata_impl.h"

int argdata_seq_iterate(const argdata_t *ad, argdata_seq_iterator_t *it_) {
  struct argdata_seq_iterator_impl *it =
      (struct argdata_seq_iterator_impl *)it_;
  switch (ad->type) {
    case AD_BUFFER:
      it->buf = ad->buffer;
      it->len = ad->length;
      it->error = parse_type(ADT_SEQ, &it->buf, &it->len);
      it->entries = NULL;
      break;
    case AD_SEQ:
      it->entries = ad->seq.entries;
      it->len = ad->seq.count;
      it->error = 0;
      break;
    default:
      it->error = EINVAL;
      break;
  }
  if (it->error != 0) {
    // If the iterator is invalid, set len to zero so that calls to
    // argdata_seq_next() act as if iterating an empty sequence.
    it->len = 0;
  }
  return it->error;
}
