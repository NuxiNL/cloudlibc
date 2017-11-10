// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>
#include <common/stdio.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

size_t fread(void *restrict ptr, size_t size, size_t nitems,
             FILE *restrict stream) {
  // Check for overflow of size * nitems.
  flockfile_orientation(stream, -1);
  size_t outbuflen;
  if (mul_overflow(size, nitems, &outbuflen)) {
    stream->flags |= F_ERROR;
    funlockfile(stream);
    errno = EINVAL;
    return 0;
  }

  // Zero-sized read. Return immediately.
  if (outbuflen == 0) {
    funlockfile(stream);
    return 0;
  }

  char *outbuf = ptr;
  for (;;) {
    // Obtain the read buffer.
    const char *readbuf;
    size_t readbuflen;
    if (!fread_peek(stream, &readbuf, &readbuflen) || readbuflen == 0) {
      funlockfile(stream);
      return (outbuf - (char *)ptr) / size;
    }

    // Last buffer. Consume it, likely partially, and return.
    if (outbuflen <= readbuflen) {
      memcpy(outbuf, readbuf, outbuflen);
      fread_consume(stream, outbuflen);
      funlockfile(stream);
      return nitems;
    }

    // Fully consume the read buffer and obtain the next one.
    memcpy(outbuf, readbuf, readbuflen);
    outbuf += readbuflen;
    outbuflen -= readbuflen;
    fread_consume(stream, readbuflen);
  }
}
