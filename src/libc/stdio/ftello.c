// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <assert.h>
#include <stdio.h>

off_t ftello(FILE *stream) {
  flockfile(stream);
  if (stream->offset == -1) {
    // File descriptor is not seekable.
    funlockfile(stream);
    errno = ESPIPE;
    return -1;
  }
  // Apply lengths of read and write buffers on top of the descriptor offset.
  assert((stream->readbuflen == 0 || stream->writebuflen == 0) &&
         "Read and write buffer both used on a streamable file");
  off_t result =
      stream->offset - (off_t)stream->readbuflen - (off_t)stream->writebuflen;
  assert(result >= 0 && "Negative offset after buffer size correction");
  funlockfile(stream);
  return result;
}
