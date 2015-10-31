// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int fseeko(FILE *stream, off_t offset, int whence) {
  flockfile(stream);
  bool result;
  if (!fseekable(stream)) {
    errno = ESPIPE;
    result = false;
  } else {
    result = fop_seek(stream, offset, whence);
  }
  if (result)
    stream->flags &= ~F_EOF;
  funlockfile(stream);
  return result ? 0 : -1;
}

#if _LONG_BIT == 64
__strong_reference(fseeko, fseek);
#endif
