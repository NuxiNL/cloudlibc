// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#include "argdata_writer_impl.h"

argdata_writer_t *argdata_writer_create(void) {
  return calloc(1, sizeof(argdata_writer_t));
}
