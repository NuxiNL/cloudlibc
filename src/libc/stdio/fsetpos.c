// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

int fsetpos(FILE *stream, const fpos_t *pos) {
  flockfile(stream);
  int result = fseeko(stream, pos->__offset, SEEK_SET);
  if (result == 0) {
    // Restore multibyte read state.
    static_assert(sizeof(pos->__mbstate) >= sizeof(stream->readstate),
                  "Multibyte read state too large");
    memcpy(&stream->readstate, &pos->__mbstate, sizeof(stream->readstate));
  }
  funlockfile(stream);
  return result;
}
