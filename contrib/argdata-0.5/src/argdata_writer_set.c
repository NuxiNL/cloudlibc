// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>

#include "argdata_writer_impl.h"

void argdata_writer_set(argdata_writer_t *as, const argdata_t *next) {
  assert(
      as->data_size == 0 &&
      "Attempted to send message, while the previous one is still in-flight");
  assert(as->next == NULL &&
         "Attempted to send message, while the previous one is still enqueued");
  as->next = next;
}
