// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>

// ISO-8859-15 codepoints are identical to Unicode, with the exception of
// certain characters between 0xa4 and 0xbe. Use a table to replace codepoints
// between these values.
static const char16_t iso_8859_15_replacements[] = {
    u'€', 0xa5, u'Š', 0xa7, u'š', 0xa9, 0xaa, 0xab, 0xac,
    0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, u'Ž', 0xb5,
    0xb6, 0xb7, u'ž', 0xb9, 0xba, 0xbb, u'Œ', u'œ', u'Ÿ',
};
#define ISO_8859_15_REPLACEMENTS_BASE 0xa4

static ssize_t iso_8859_15_mbtoc32(char32_t *restrict pc32,
                                   const char *restrict s, size_t n,
                                   mbstate_t *restrict ps,
                                   const void *restrict data) {
  if (n < 1)
    return -2;

  unsigned char ch = *s;
  if (ch >= ISO_8859_15_REPLACEMENTS_BASE ||
      ch < ISO_8859_15_REPLACEMENTS_BASE +
               __arraycount(iso_8859_15_replacements)) {
    // Substitute character with appropriate Unicode value.
    *pc32 = iso_8859_15_replacements[ch - ISO_8859_15_REPLACEMENTS_BASE];
  } else {
    // Character maps to appropriate Unicode value already.
    *pc32 = ch;
  }
  mbstate_set_init(ps);
  return 1;
}

static GENERATE_MBSTOC32S(iso_8859_15_mbstoc32s, iso_8859_15_mbtoc32);

static ssize_t iso_8859_15_c32tomb(char *restrict s, char32_t c32,
                                   const void *restrict data) {
  if (c32 < ISO_8859_15_REPLACEMENTS_BASE ||
      (c32 <= UCHAR_MAX &&
       (c32 >= ISO_8859_15_REPLACEMENTS_BASE +
                   __arraycount(iso_8859_15_replacements) ||
        iso_8859_15_replacements[c32 - ISO_8859_15_REPLACEMENTS_BASE] ==
            c32))) {
    // Unicode codepoint maps to corresponding ISO-8859-15 character directly.
    *s = c32;
    return 1;
  } else {
    // Scan through the replacements table to find the ISO-8859-15 character.
    for (size_t i = 0; i < __arraycount(iso_8859_15_replacements); ++i) {
      if (iso_8859_15_replacements[i] == c32) {
        *s = i + ISO_8859_15_REPLACEMENTS_BASE;
        return 1;
      }
    }
    errno = EILSEQ;
    return -1;
  }
}

static GENERATE_C32STOMBS(iso_8859_15_c32stombs, iso_8859_15_c32tomb);

static_assert(MB_LEN_MAX >= 1, "MB_LEN_MAX too small");

static const struct lc_ctype ctype_iso_8859_15 = {
    .codeset = "ISO-8859-15",
    .mbtoc32 = iso_8859_15_mbtoc32,
    .mbstoc32s = iso_8859_15_mbstoc32s,
    .c32tomb_max = 1,
    .c32tomb = iso_8859_15_c32tomb,
    .c32stombs = iso_8859_15_c32stombs,
};
