// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <unistd.h>

#include "argdata_reader_impl.h"

void argdata_reader_free(argdata_reader_t *ar) {
  for (size_t i = 0; i < ar->fds_len; ++i)
    if (ar->fds[i] >= 0)
      close(ar->fds[i]);
  free(ar);
}
