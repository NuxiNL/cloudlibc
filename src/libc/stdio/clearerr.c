// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

void clearerr(FILE *stream) {
  flockfile(stream);
  stream->flags &= ~(F_EOF | F_ERROR);
  funlockfile(stream);
}
