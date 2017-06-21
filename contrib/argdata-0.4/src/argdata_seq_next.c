// Copyright (c) 2016-2017 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include "argdata_impl.h"

void argdata_seq_next(argdata_seq_iterator_t *it_) {
  struct argdata_seq_iterator_impl *it =
      (struct argdata_seq_iterator_impl *)it_;
  if (it->type == ADS_BUFFER) {
    if (it->buffer.length == 0) {
      it->type = ADS_SEQ;
      it->seq.count = 0;
      it->index = ARGDATA_ITERATOR_END;
    } else {
      if (parse_subfield(&it->buffer.entry, &it->buffer.buffer,
                         &it->buffer.length, it->buffer.convert_fd,
                         it->buffer.convert_fd_arg) != 0) {
        // Failed to load next sequence entry. Return an empty sequence.
        it->type = ADS_SEQ;
        it->seq.count = 0;
        it->index = ARGDATA_ITERATOR_INVALID;
      } else {
        ++it->index;
      }
    }
  } else {  // type == ADS_SEQ
    if (it->seq.count > 0) {
      ++it->seq.entries;
      --it->seq.count;
      ++it->index;
    } else {
      it->index = ARGDATA_ITERATOR_END;
    }
  }
}
