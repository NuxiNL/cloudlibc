// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
  off_t offset = ftello_logical(stream);
  if (offset < 0) {
    // Don't attempt to preserve negative offsets in an fpos_t, as there
    // is no meaningful way in which they can be used with fsetpos().
    funlockfile(stream);
    errno = EOVERFLOW;
    return -1;
  }
  pos->__offset = offset;
  static_assert(sizeof(pos->__mbstate) >= sizeof(stream->readstate),
                "Multibyte read state too large");
  memcpy(&pos->__mbstate, &stream->readstate, sizeof(stream->readstate));
  funlockfile(stream);
  return 0;
}
