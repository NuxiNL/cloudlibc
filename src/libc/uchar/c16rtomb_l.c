// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <errno.h>
#include <limits.h>
#include <uchar.h>

struct c16tombstate {
  char16_t lead_surrogate;
};

size_t c16rtomb_l(char *restrict s, char16_t c16, mbstate_t *restrict ps,
                  locale_t locale) {
  char buf[MB_LEN_MAX];
  if (s == NULL) {
    // No output buffer.
    s = buf;
    c16 = u'\0';
  }

  struct c16tombstate *cs = (struct c16tombstate *)ps;
  char32_t c32;
  if (cs->lead_surrogate >= 0xd800 && cs->lead_surrogate <= 0xdbff) {
    // We should see a trail surrogate to complete this surrogate pair.
    if (c16 < 0xdc00 || c16 > 0xdfff) {
      errno = EILSEQ;
      return (size_t)-1;
    }
    c32 = 0x10000 + ((cs->lead_surrogate & 0x3ff) << 10 | (c16 & 0x3ff));
  } else if (c16 >= 0xd800 && c16 <= 0xdbff) {
    // Start of a surrogate pair. Store this lead surrogate.
    cs->lead_surrogate = c16;
    return 0;
  } else {
    // Regular UTF-16 character.
    c32 = c16;
  }

  // Convert UTF-32 character.
  const struct lc_ctype *ctype = locale->ctype;
  ssize_t l = ctype->c32tomb(s, c32, ctype->data);
  if (l >= 0)
    cs->lead_surrogate = 0;
  return l;
}
