// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int fseeko(FILE *stream, off_t offset, int whence) {
  flockfile(stream);
  bool result = stream->ops->seek(stream, offset, whence);
  if (result) {
    funlockfile(stream);
    return 0;
  } else {
    stream->flags |= F_ERROR;
    funlockfile(stream);
    return -1;
  }
}
