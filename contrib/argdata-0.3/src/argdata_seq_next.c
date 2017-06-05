// Copyright (c) 2016-2017 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "argdata_impl.h"

bool argdata_seq_next(argdata_seq_iterator_t *it_, const argdata_t **value) {
  struct argdata_seq_iterator_impl *it =
      (struct argdata_seq_iterator_impl *)it_;
  if (it->len == 0)
    return false;
  if (it->entries == NULL) {
    int error = parse_subfield(&it->value, &it->buf, &it->len, it->convert_fd,
                               it->convert_fd_arg);
    if (error != 0) {
      it->error = error;
      return false;
    }
    *value = &it->value;
    return true;
  } else {
    *value = *it->entries++;
    --it->len;
    return true;
  }
}
