// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int fflush(FILE *stream) {
  flockfile(stream);
  bool result = fop_flush(stream);
  funlockfile(stream);
  return result ? 0 : EOF;
}
