// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>
#include <common/stdio.h>

#include <stdint.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>

int fputws(const wchar_t *restrict ws, FILE *restrict stream) {
  // For consistency with fputs(), leave the stream unaffected if the
  // string is empty.
  const char32_t *c32s = (const char32_t *)ws;
  if (*c32s == U'\0')
    return 0;

  // Only allow action if we're opened for writing.
  flockfile(stream);
  if ((stream->oflags & O_WRONLY) == 0) {
    stream->flags |= F_ERROR;
    funlockfile(stream);
    errno = EBADF;
    return WEOF;
  }

  while (*c32s != U'\0') {
    // Obtain a write buffer.
    if (stream->writebuflen == 0) {
      if (!fop_write_peek(stream)) {
        stream->flags |= F_ERROR;
        funlockfile(stream);
        return WEOF;
      }
    }

    // If the remaining space in the write buffer is smaller than
    // MB_LEN_MAX, we won't be able to let c32tomb() or c32stombs()
    // store its result in the write buffer directly. The resulting
    // character may span the buffer boundary. Perform a simple putwc()
    // for this character.
    if (stream->writebuflen < MB_LEN_MAX) {
      if (putwc_unlocked(*c32s++, stream) == WEOF) {
        funlockfile(stream);
        return WEOF;
      }
      continue;
    }

    // Call into c32stombs() to convert multiple wide characters in one
    // go. The result will be written in the write buffer directly.
    // TODO(ed): Honour buffering mechanism.
    const struct lc_ctype *ctype = stream->ctype;
    ssize_t len = ctype->c32stombs(stream->writebuf, &c32s, SIZE_MAX,
                                   stream->writebuflen, ctype->data);
    if (len == -1) {
      stream->flags |= F_ERROR;
      funlockfile(stream);
      return WEOF;
    }
    stream->writebuf += len;
    stream->writebuflen -= len;
    if (c32s == NULL)
      break;
  }
  funlockfile(stream);
  return 0;
}
