// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int fflush(FILE *stream) {
  // TODO(ed): This is wrong?
  if ((stream->oflags & O_WRONLY) == 0)
    return 0;

  flockfile(stream);
  bool result = fop_write_flush(stream);
  funlockfile(stream);
  return result ? 0 : EOF;
}
