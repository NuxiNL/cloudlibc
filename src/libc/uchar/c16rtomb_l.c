// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <errno.h>
#include <limits.h>
#include <uchar.h>

size_t c16rtomb_l(char *restrict s, char16_t c16, mbstate_t *restrict ps,
                  locale_t locale) {
  char buf[MB_LEN_MAX];
  if (s == NULL) {
    // No output buffer.
    s = buf;
    c16 = u'\0';
  }

  char32_t c32;
  char16_t c16lead;
  if (mbstate_get_surrogate(ps, &c16lead)) {
    // We should see a trail surrogate to complete this surrogate pair.
    if (c16 < 0xdc00 || c16 > 0xdfff) {
      errno = EILSEQ;
      return (size_t)-1;
    }
    c32 = 0x10000 + ((c16lead & 0x3ff) << 10 | (c16 & 0x3ff));
  } else if (c16 >= 0xd800 && c16 <= 0xdbff) {
    // Start of a surrogate pair. Store this lead surrogate.
    mbstate_set_surrogate(ps, c16);
    return 0;
  } else {
    // Regular UTF-16 character.
    c32 = c16;
  }

  // Convert UTF-32 character.
  const struct lc_ctype *ctype = locale->ctype;
  ssize_t l = ctype->c32tomb(s, c32, ctype->data);
  if (l >= 0)
    mbstate_set_init(ps);
  return l;
}
