// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int fgetpos(FILE *restrict stream, fpos_t *restrict pos) {
  flockfile(stream);
  if (!fseekable(stream)) {
    // File descriptor is not seekable.
    funlockfile(stream);
    errno = ESPIPE;
    return -1;
  }
  pos->__offset = ftello_logical(stream);
  static_assert(sizeof(pos->__mbstate) >= sizeof(stream->readstate),
                "Multibyte read state too large");
  memcpy(&pos->__mbstate, &stream->readstate, sizeof(stream->readstate));
  funlockfile(stream);
  return 0;
}
