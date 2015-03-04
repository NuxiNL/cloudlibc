// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

int fgetpos(FILE *restrict stream, fpos_t *restrict pos) {
  // Obtain file offset.
  flockfile(stream);
  off_t result = ftello(stream);
  if (result == -1) {
    funlockfile(stream);
    return -1;
  }

  // Preserve offset and multibyte read state.
  pos->__offset = result;
  static_assert(sizeof(pos->__mbstate) >= sizeof(stream->readstate),
                "Multibyte read state too large");
  memcpy(&pos->__mbstate, &stream->readstate, sizeof(stream->readstate));
  funlockfile(stream);
  return 0;
}
