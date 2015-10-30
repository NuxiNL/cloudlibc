// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int setvbuf(FILE *restrict stream, char *restrict buf, int type, size_t size) {
  flockfile(stream);
  bool result = fop_setvbuf(stream, size);
  if (result)
    stream->buftype = type;
  funlockfile(stream);
  return !result;
}
