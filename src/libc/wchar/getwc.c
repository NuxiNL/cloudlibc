// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/stdio.h>

#include <sys/types.h>

#include <uchar.h>
#include <wchar.h>

wint_t getwc(FILE *stream) {
  flockfile_orientation(stream, 1);
  const struct lc_ctype *ctype = stream->ctype;
  for (;;) {
    // Obtain read buffer.
    const char *readbuf;
    size_t readbuflen;
    if (!fread_peek(stream, &readbuf, &readbuflen) || readbuflen == 0) {
      funlockfile(stream);
      return WEOF;
    }
    // Pass entire read buffer to mbtoc32().
    char32_t c32;
    ssize_t len = ctype->mbtoc32(&c32, readbuf, readbuflen, &stream->readstate,
                                 ctype->data);
    if (len == -1) {
      // Conversion error.
      stream->flags |= F_ERROR;
      funlockfile(stream);
      return WEOF;
    }
    fread_consume(stream, len);
    if (len != -2) {
      // Parsed a full character.
      funlockfile(stream);
      return c32;
    }
  }
}

__strong_reference(getwc, fgetwc);
