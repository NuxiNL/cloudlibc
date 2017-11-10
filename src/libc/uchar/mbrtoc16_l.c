// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <uchar.h>

size_t mbrtoc16_l(char16_t *restrict pc16, const char *restrict s, size_t n,
                  mbstate_t *restrict ps, locale_t locale) {
  char16_t c16;
  if (s == NULL) {
    // No input buffer.
    pc16 = &c16;
    s = "";
    n = 1;
  } else if (pc16 == NULL) {
    // Discard character.
    pc16 = &c16;
  }

  // Return trail surrogate if available.
  if (mbstate_get_surrogate(ps, pc16)) {
    mbstate_set_init(ps);
    return (size_t)-3;
  }

  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  ssize_t l = ctype->mbtoc32(&c32, s, n, ps, ctype->data);
  if (l >= 0) {
    if (c32 < 0x10000) {
      // Also a valid UTF-16 character.
      *pc16 = c32;
    } else {
      // Split character up in surrogate pair. Return the lead surrogate
      // and return the trail surrogate.
      c32 -= 0x10000;
      *pc16 = 0xd800 | (c32 >> 10);
      mbstate_set_surrogate(ps, 0xdc00 | (c32 & 0x3ff));
    }
    if (*pc16 == u'\0')
      return 0;
  }
  return l;
}
