// Copyright (c) 2016-2017 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include "argdata_impl.h"

void argdata_map_next(argdata_map_iterator_t *it_) {
  struct argdata_map_iterator_impl *it =
      (struct argdata_map_iterator_impl *)it_;
  if (it->type == ADM_BUFFER) {
    if (it->buffer.length == 0) {
      it->index = (size_t)-1;
      it->type = ADM_MAP;
      it->map.count = 0;
    } else {
      if (parse_subfield(&it->buffer.key, &it->buffer.buffer,
                         &it->buffer.length, it->buffer.convert_fd,
                         it->buffer.convert_fd_arg) != 0 ||
          parse_subfield(&it->buffer.value, &it->buffer.buffer,
                         &it->buffer.length, it->buffer.convert_fd,
                         it->buffer.convert_fd_arg) != 0) {
        // Failed to load next map entry. Return an empty map.
        it->type = ADM_MAP;
        it->map.count = 0;
        it->index = ARGDATA_ITERATOR_INVALID;
      } else {
        ++it->index;
      }
    }
  } else {  // type == ADM_MAP
    if (it->map.count > 0) {
      ++it->map.keys;
      ++it->map.values;
      --it->map.count;
      ++it->index;
    } else {
      it->index = ARGDATA_ITERATOR_END;
    }
  }
}
