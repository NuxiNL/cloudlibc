// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <stddef.h>

#include "argdata_reader_impl.h"

const argdata_t *argdata_reader_get(const argdata_reader_t *ar) {
  assert(ar->finished && "Attempted to extract message, while still in-flight");
  assert((ar->header_len == 0 || ar->header_len == sizeof(ar->header)) &&
         "Cannot have a partially read header");
  return ar->header_len == 0 ? NULL : &ar->root;
}
