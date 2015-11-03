// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>
#include <common/stdio.h>

#include <limits.h>
#include <stdio.h>
#include <wchar.h>

wint_t putwc(wchar_t wc, FILE *stream) {
  // Convert character to a multibyte sequence, using the character set
  // associated with the stream.
  flockfile(stream);
  const struct lc_ctype *ctype = stream->ctype;
  char buf[MB_LEN_MAX];
  ssize_t len = ctype->c32tomb(buf, wc, ctype->data);
  if (len == -1) {
    stream->flags |= F_ERROR;
    funlockfile(stream);
    return WEOF;
  }

  // Write it.
  size_t written = fwrite_put(stream, buf, len);
  funlockfile(stream);
  return written == (size_t)len ? wc : WEOF;
}

__strong_reference(putwc, fputwc);
