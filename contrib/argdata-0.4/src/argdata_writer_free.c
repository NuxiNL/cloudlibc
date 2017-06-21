// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#include "argdata_writer_impl.h"

void argdata_writer_free(argdata_writer_t *aw) {
  free(aw->control);
  free(aw);
}
