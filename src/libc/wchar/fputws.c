// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/stdio.h>

#include <stdint.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>

int fputws(const wchar_t *restrict ws, FILE *restrict stream) {
  // For consistency with fputs(), leave the stream unaffected if the
  // string is empty.
  flockfile_orientation(stream, 1);
  size_t len = wcslen(ws);
  if (len == 0) {
    funlockfile(stream);
    return 0;
  }

  const struct lc_ctype *ctype = stream->ctype;
  const char32_t *c32s = (const char32_t *)ws;
  const char32_t *end = c32s + len;
  do {
    // Obtain a write buffer.
    char *writebuf;
    size_t writebuflen;
    if (!fwrite_peek(stream, &writebuf, &writebuflen)) {
      funlockfile(stream);
      return -1;
    }
    if (writebuflen < MB_LEN_MAX) {
      // If the remaining space in the write buffer is smaller than
      // MB_LEN_MAX, we won't be able to let c32tomb() or c32stombs()
      // store its result in the write buffer directly. The resulting
      // character may span the buffer boundary. Perform a simple
      // putwc() for this character.
      if (putwc_unlocked(*c32s++, stream) == WEOF) {
        funlockfile(stream);
        return -1;
      }
    } else {
      // Call into c32stombs() to convert multiple wide characters in
      // one go. The result will be written in the write buffer directly.
      ssize_t written = ctype->c32stombs(writebuf, writebuflen, &c32s,
                                         end - c32s, ctype->data);
      if (written == -1) {
        stream->flags |= F_ERROR;
        funlockfile(stream);
        return -1;
      }
      fwrite_produce(stream, written);
    }
  } while (c32s < end);
  funlockfile(stream);
  return 0;
}
