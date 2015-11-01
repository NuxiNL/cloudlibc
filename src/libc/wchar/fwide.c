// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <wchar.h>

int fwide(FILE *stream, int mode) {
  flockfile(stream);
  if (stream->orientation == 0 && mode != 0)
    stream->orientation = mode > 0 ? 1 : -1;
  int result = stream->orientation;
  funlockfile(stream);
  return result;
}
