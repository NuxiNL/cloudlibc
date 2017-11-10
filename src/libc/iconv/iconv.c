// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <iconv.h>
#include <limits.h>
#include <string.h>

#include "iconv_impl.h"

size_t iconv(iconv_t cd, char **restrict inbuf, size_t *restrict inbytesleft,
             char **restrict outbuf, size_t *restrict outbytesleft) {
  // Switch back to initial conversion state.
  if (inbuf == NULL || *inbuf == NULL) {
    mbstate_set_init(&cd->from_state);
    return 0;
  }

  bool have_ilseq = false;
  while (*inbytesleft > 0) {
    // Convert next character to Unicode.
    char32_t c32;
    const struct lc_ctype *from = cd->from;
    mbstate_t from_state = cd->from_state;
    ssize_t inlen =
        from->mbtoc32(&c32, *inbuf, *inbytesleft, &from_state, from->data);
    if (inlen == -1) {
      // Invalid character.
      if (!cd->ignore)
        return (size_t)-1;
      have_ilseq = true;

      // Skip offending byte and reset to the initial conversion state.
      mbstate_set_init(&cd->from_state);
      ++*inbuf;
      --*inbytesleft;
      continue;
    } else if (inlen == -2) {
      // Attempted to convert a character partially.
      errno = EINVAL;
      return (size_t)-1;
    }

    // Convert Unicode back to multibyte.
    const struct lc_ctype *to = cd->to;
    char buf[MB_LEN_MAX];
    ssize_t outlen = to->c32tomb(buf, c32, to->data);
    if (outlen == -1) {
      // Invalid character.
      if (!cd->ignore)
        return (size_t)-1;
      have_ilseq = true;
    } else if ((size_t)outlen > *outbytesleft) {
      // Character too big for output buffer.
      errno = E2BIG;
      return (size_t)-1;
    } else {
      // Valid output character.
      memcpy(*outbuf, buf, outlen);
      *outbuf += outlen;
      *outbytesleft -= outlen;
    }

    // Progress input buffer.
    cd->from_state = from_state;
    *inbuf += inlen;
    *inbytesleft -= inlen;
  }

  // The //IGNORE flag delays the reporting of encoding errors.
  if (have_ilseq) {
    errno = EILSEQ;
    return (size_t)-1;
  }
  return 0;
}
