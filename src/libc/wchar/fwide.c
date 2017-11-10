// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
