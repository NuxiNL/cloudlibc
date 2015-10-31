// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdbool.h>
#include <stdio.h>

int ungetc(int c, FILE *stream) {
  // Pushing EOF is not permitted.
  if (c == EOF)
    return EOF;

  // Push byte into ungetc buffer. Only allow this to be performed if
  // the stream has been opened for reading.
  flockfile(stream);
  bool result = false;
  if ((stream->oflags & O_RDONLY) != 0 &&
      stream->ungetclen < sizeof(stream->ungetc)) {
    stream->ungetc[sizeof(stream->ungetc) - ++stream->ungetclen] = c;
    stream->flags &= ~F_EOF;
    result = true;
  }
  funlockfile(stream);
  return result ? (unsigned char)c : EOF;
}
