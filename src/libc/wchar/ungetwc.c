// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

wint_t ungetwc(wint_t wc, FILE *stream) {
  // Push character into ungetc buffer after multibyte encoding. Only
  // allow this to be performed if the stream has been opened for
  // reading. According to the standard, this function may set errno to
  // EILSEQ. Set errno to reasonable values for all other error cases.
  flockfile_orientation(stream, 1);
  bool result = false;
  if (wc == WEOF) {
    // Pushing WEOF is not permitted.
    errno = EINVAL;
  } else {
    const struct lc_ctype *ctype = stream->ctype;
    char buf[MB_LEN_MAX];
    ssize_t len = ctype->c32tomb(buf, wc, ctype->data);
    if (len != -1) {
      if (stream->ungetclen + len > sizeof(stream->ungetc)) {
        errno = ENOSPC;
      } else {
        stream->ungetclen += len;
        memcpy(stream->ungetc + sizeof(stream->ungetc) - stream->ungetclen, buf,
               len);
        stream->flags &= ~F_EOF;
        result = true;
      }
    }
  }
  funlockfile(stream);
  return result ? wc : WEOF;
}
