// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/stdio.h>

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

wchar_t *fgetws(wchar_t *restrict ws, int n, FILE *restrict stream) {
  // Handle cases where we don't need to access any data.
  flockfile_orientation(stream, 1);
  if (n <= 0) {
    funlockfile(stream);
    return NULL;
  } else if (n == 1) {
    funlockfile(stream);
    *ws = L'\0';
    return ws;
  }

  const struct lc_ctype *ctype = stream->ctype;
  char32_t *outbuf = (char32_t *)ws;
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
      if (outbuf == (char32_t *)ws)
        return NULL;
      *outbuf = U'\0';
      return ws;
    }

    // Limit the size of the read buffer up to the first newline
    // character. As we don't want to perform excessive scanning of the
    // read buffer for short reads, first limit the read buffer to a
    // safe upperbound. Assume that wide character text has up to two
    // bytes per character on average.
    //
    // Assume that for the character encoding a newline is always
    // encoded as '\n'.
    size_t upperbound = (MB_LEN_MAX > outbuflen ? MB_LEN_MAX : outbuflen) * 2;
    if (readbuflen > upperbound)
      readbuflen = upperbound;
    const char *newline = memchr(readbuf, '\n', readbuflen);
    if (newline != NULL)
      readbuflen = newline - readbuf + 1;

    // Call into mbstoc32s() to convert read buffer contents to wide
    // characters.
    const char *new_readbuf = readbuf;
    ssize_t len = ctype->mbstoc32s(outbuf, outbuflen, &new_readbuf, readbuflen,
                                   &stream->readstate, ctype->data, true);
    if (len == -1) {
      // Conversion error.
      stream->flags |= F_ERROR;
      funlockfile(stream);
      return NULL;
    }
    fread_consume(stream, new_readbuf - readbuf);
    if (newline != NULL || (size_t)len == outbuflen) {
      // Successfully read a line or filled the entire output buffer.
      funlockfile(stream);
      outbuf[len] = U'\0';
      return ws;
    }
    outbuf += len;
    outbuflen -= len;
  }
}
