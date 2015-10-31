// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <assert.h>
#include <stdio.h>

off_t ftello(FILE *stream) {
  flockfile(stream);
  if (!fseekable(stream)) {
    // File descriptor is not seekable.
    funlockfile(stream);
    errno = ESPIPE;
    return -1;
  }
  // Apply lengths of the read, write and ungetc buffers on top of the
  // descriptor offset.
  off_t result = ftello_fast(stream) - stream->ungetclen;
  funlockfile(stream);
  return result;
}

#if _LONG_BIT == 64
__strong_reference(ftello, ftell);
#endif
