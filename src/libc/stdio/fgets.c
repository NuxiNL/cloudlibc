// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>
#include <string.h>

char *fgets(char *restrict s, int n, FILE *restrict stream) {
  // Handle cases where we don't need to access any data.
  flockfile_orientation(stream, -1);
  if (n <= 0) {
    funlockfile(stream);
    return NULL;
  } else if (n == 1) {
    funlockfile(stream);
    *s = '\0';
    return s;
  }

  char *outbuf = s;
  size_t outbuflen = n - 1;
  for (;;) {
    // Obtain the read buffer.
    const char *readbuf;
    size_t readbuflen;
    if (!fread_peek(stream, &readbuf, &readbuflen)) {
      funlockfile(stream);
      return NULL;
    }

    // End-of-file. Return the buffer or NULL, depending on whether
    // we've already yielded any output.
    if (readbuflen == 0) {
      funlockfile(stream);
      if (outbuf == s)
        return NULL;
      *outbuf = '\0';
      return s;
    }

    // Limit the size of the output buffer if we find a newline
    // character to stop reading at that point.
    const char *newline =
        memchr(readbuf, '\n', readbuflen < outbuflen ? readbuflen : outbuflen);
    if (newline != NULL)
      outbuflen = newline - readbuf + 1;

    // Last buffer. Consume it, likely partially, and return.
    if (outbuflen <= readbuflen) {
      memcpy(outbuf, readbuf, outbuflen);
      fread_consume(stream, outbuflen);
      funlockfile(stream);
      outbuf[outbuflen] = '\0';
      return s;
    }

    // Fully consume the read buffer and obtain the next one.
    memcpy(outbuf, readbuf, readbuflen);
    outbuf += readbuflen;
    outbuflen -= readbuflen;
    fread_consume(stream, readbuflen);
  }
}
