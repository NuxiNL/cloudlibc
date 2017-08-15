// Copyright (c) 2016-2017 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>

#include "argdata_impl.h"

void argdata_seq_iterate(const argdata_t *ad, argdata_seq_iterator_t *it_) {
  struct argdata_seq_iterator_impl *it =
      (struct argdata_seq_iterator_impl *)it_;
  switch (ad->type) {
    case AD_BUFFER:
      // Buffer based sequence iterator.
      it->buffer.buffer = ad->buffer.buffer;
      it->buffer.length = ad->length;
      it->buffer.convert_fd = ad->buffer.convert_fd;
      it->buffer.convert_fd_arg = ad->buffer.convert_fd_arg;
      if (parse_type(ADT_SEQ, &it->buffer.buffer, &it->buffer.length) != 0) {
        // Not a sequence. Return an empty sequence.
        it->type = ADS_SEQ;
        it->seq.count = 0;
        it->index = ARGDATA_ITERATOR_INVALID;
      } else {
        // Call into argdata_seq_next() to load the first entry.
        it->type = ADS_BUFFER;
        it->index = (size_t)-1;  // Will get incremented to 0, if nonempty.
        argdata_seq_next(it_);
      }
      break;
    case AD_SEQ:
      // Memory based sequence iterator.
      it->type = ADS_SEQ;
      it->seq.entries = ad->seq.entries;
      it->seq.count = ad->seq.count;
      it->index = 0;
      break;
    default:
      // Not a sequence. Return an empty sequence.
      it->type = ADS_SEQ;
      it->seq.count = 0;
      it->index = ARGDATA_ITERATOR_INVALID;
      break;
  }
}
