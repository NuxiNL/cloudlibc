// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/overflow.h>
#include <common/stdio.h>

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

int fseeko(FILE *stream, off_t offset, int whence) {
  flockfile(stream);
  bool result = false;
  if (!fseekable(stream)) {
    // Attempted to call fseeko() on a non-seekable stream.
    errno = ESPIPE;
  } else {
    // Process whence value.
    switch (whence) {
      case SEEK_CUR:
        // Add the current position and perform SEEK_SET.
        if (add_overflow(offset, ftello_logical(stream), &offset)) {
          errno = EOVERFLOW;
          break;
        }
        result = fop_seek(stream, offset, false);
        break;
      case SEEK_END:
        result = fop_seek(stream, offset, true);
        break;
      case SEEK_SET:
        result = fop_seek(stream, offset, false);
        break;
      default:
        errno = EINVAL;
        break;
    }
    if (result) {
      stream->flags &= ~F_EOF;
      stream->ungetclen = 0;
      memset(&stream->readstate, '\0', sizeof(stream->readstate));
    }
  }
  funlockfile(stream);
  return result ? 0 : -1;
}

#if LONG_BIT == 64
__strong_reference(fseeko, fseek);
#endif
