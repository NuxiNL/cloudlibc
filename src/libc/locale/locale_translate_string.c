// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

size_t __locale_translate_string(locale_t locale, char *dst, const char *src,
                                 size_t dstsize) {
  const struct lc_ctype *from = &__ctype_utf_8;
  const struct lc_ctype *to = locale->ctype;

  // Decode/encode characters one by one.
  size_t total = 1;
  mbstate_t mbs;
  mbstate_set_init(&mbs);
  while (*src != '\0') {
    char32_t ch;
    ssize_t len = from->mbtoc32(&ch, src, SIZE_MAX, &mbs, from->data);
    assert(len > 0 && "Failed to decode compile-time string");
    src += len;

    // Substitute character by a question mark if it cannot be encoded.
    char buf[MB_LEN_MAX];
    len = to->c32tomb(buf, ch, to->data);
    if (len < 0) {
      buf[0] = '?';
      len = 1;
    }

    // Only put it in the output buffer if we can fully fit it.
    if ((size_t)len < dstsize) {
      memcpy(dst, buf, len);
      dst += len;
      dstsize -= len;
    }
    total += len;
  }

  if (dstsize > 0)
    *dst = '\0';
  return total;
}
