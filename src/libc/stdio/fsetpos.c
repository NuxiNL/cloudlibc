// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int fsetpos(FILE *stream, const fpos_t *pos) {
  flockfile(stream);
  bool result = false;
  if (!fseekable(stream)) {
    // Attempted to call fsetpos() on a non-seekable stream.
    errno = ESPIPE;
  } else {
    result = fop_seek(stream, pos->__offset, false);
    if (result) {
      stream->flags &= ~F_EOF;
      stream->ungetclen = 0;
      memset(&stream->readstate, '\0', sizeof(stream->readstate));
      static_assert(sizeof(pos->__mbstate) >= sizeof(stream->readstate),
                    "Multibyte read state too large");
      memcpy(&stream->readstate, &pos->__mbstate, sizeof(stream->readstate));
    }
  }
  funlockfile(stream);
  return result ? 0 : -1;
}
