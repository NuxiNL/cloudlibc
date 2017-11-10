// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdbool.h>
#include <stdio.h>

void rewind(FILE *stream) {
  flockfile(stream);
  if (fseekable(stream))
    fop_seek(stream, 0, false);
  stream->flags &= ~(F_EOF | F_ERROR);
  stream->ungetclen = 0;
  memset(&stream->readstate, '\0', sizeof(stream->readstate));
  funlockfile(stream);
}
