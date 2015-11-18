// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <uchar.h>

struct mbtoc16state {
  char16_t trail_surrogate;
  struct mbtoc32state c32state;
};

static_assert(sizeof(struct mbtoc16state) <= sizeof(__mbstate_t),
              "mbstate_t has become too small");

size_t mbrtoc16_l(char16_t *restrict pc16, const char *restrict s, size_t n,
                  mbstate_t *restrict ps, locale_t locale) {
  // Return trail surrogate if available.
  struct mbtoc16state *cs = (struct mbtoc16state *)ps;
  if (cs->trail_surrogate >= 0xdc00 && cs->trail_surrogate <= 0xdfff) {
    *pc16 = cs->trail_surrogate;
    cs->trail_surrogate = 0;
    return (size_t)-3;
  }

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

  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  ssize_t l = ctype->mbtoc32(&c32, s, n, &cs->c32state, ctype->data);
  if (l >= 0) {
    if (c32 < 0x10000) {
      // Also a valid UTF-16 character.
      *pc16 = c32;
    } else {
      // Split character up in surrogate pair. Return the lead surrogate
      // and return the trail surrogate.
      c32 -= 0x10000;
      *pc16 = 0xd800 | (c32 >> 10);
      cs->trail_surrogate = 0xdc00 | (c32 & 0x3ff);
    }
    if (*pc16 == u'\0')
      return 0;
  }
  return l;
}
