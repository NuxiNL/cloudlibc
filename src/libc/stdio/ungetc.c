// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>

int ungetc(int c, FILE *stream) {
  // Push byte into ungetc buffer. Set errno to ENOSPC for consistency
  // with ungetwc().
  flockfile_orientation(stream, -1);
  bool result = false;
  if (c == EOF) {
    // Pushing EOF is not permitted.
    errno = EINVAL;
  } else if (stream->ungetclen >= sizeof(stream->ungetc)) {
    errno = ENOSPC;
  } else {
    stream->ungetc[sizeof(stream->ungetc) - ++stream->ungetclen] = c;
    stream->flags &= ~F_EOF;
    result = true;
  }
  funlockfile(stream);
  return result ? (unsigned char)c : EOF;
}
