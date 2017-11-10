// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>

static ssize_t utf_8_mbtoc32(char32_t *restrict pc32, const char *restrict s,
                             size_t n, mbstate_t *restrict ps,
                             const void *restrict data) {
  const unsigned char *sb = (const unsigned char *)s;

  // We need at least some data.
  if (n < 1)
    return -2;

  // Parse the leading byte in case we are in the initial state.
  unsigned int bytesleft;
  char32_t partial, lowerbound;
  mbstate_get_multibyte(ps, &bytesleft, &partial, &lowerbound);
  if (bytesleft == 0) {
    if ((*sb & 0x80) == 0) {
      // Start of 1-byte sequence.
      *pc32 = *sb;
      mbstate_set_init(ps);
      return 1;
    } else if ((*sb & 0xe0) == 0xc0) {
      // Start of 2-byte sequence.
      bytesleft = 1;
      partial = *sb++ & 0x1f;
      lowerbound = 0x80;
    } else if ((*sb & 0xf0) == 0xe0) {
      // Start of 3-byte sequence.
      bytesleft = 2;
      partial = *sb++ & 0xf;
      lowerbound = 0x800;
    } else if ((*sb & 0xf8) == 0xf0) {
      // Start of 4-byte sequence.
      bytesleft = 3;
      partial = *sb++ & 0x7;
      lowerbound = 0x10000;
    } else {
      errno = EILSEQ;
      return -1;
    }
    --n;
  }

  // Parse continuation bytes.
  while (n-- > 0) {
    if ((*sb & 0xc0) != 0x80) {
      errno = EILSEQ;
      return -1;
    }

    // Valid continuation byte.
    partial <<= 6;
    partial |= *sb++ & 0x3f;

    if (--bytesleft == 0) {
      // Ensure that the character was not overlong encoded or outside
      // of range.
      if (partial < lowerbound || (partial >= 0xd800 && partial <= 0xdfff) ||
          partial > 0x10ffff) {
        errno = EILSEQ;
        return -1;
      }
      *pc32 = partial;
      mbstate_set_init(ps);
      return sb - (const unsigned char *)s;
    }
  }
  mbstate_set_multibyte(ps, bytesleft, partial, lowerbound);
  return -2;
}

static GENERATE_MBSTOC32S(utf_8_mbstoc32s, utf_8_mbtoc32);

static ssize_t utf_8_c32tomb(char *restrict s, char32_t c32,
                             const void *restrict data) {
  if (c32 <= 0x7f) {
    // 1-byte sequence: 0_______.
    *s = c32;
    return 1;
  } else if (c32 <= 0x7ff) {
    // 2-byte sequence: 110_____ 10______.
    *s++ = 0xc0 | (c32 >> 6);
    *s = 0x80 | (c32 & 0x3f);
    return 2;
  } else if (c32 <= 0xffff) {
    // Disallow surrogate halves.
    if (c32 >= 0xd800 && c32 <= 0xdfff) {
      errno = EILSEQ;
      return -1;
    }
    // 3-byte sequence: 1110____ 10______ 10______.
    *s++ = 0xe0 | (c32 >> 12);
    *s++ = 0x80 | ((c32 >> 6) & 0x3f);
    *s = 0x80 | (c32 & 0x3f);
    return 3;
  } else if (c32 <= 0x10ffff) {
    // 4-byte sequence: 11110___ 10______ 10______ 10______.
    *s++ = 0xf0 | (c32 >> 18);
    *s++ = 0x80 | ((c32 >> 12) & 0x3f);
    *s++ = 0x80 | ((c32 >> 6) & 0x3f);
    *s = 0x80 | (c32 & 0x3f);
    return 4;
  } else {
    // Sequence out of bounds.
    errno = EILSEQ;
    return -1;
  }
}

static GENERATE_C32STOMBS(utf_8_c32stombs, utf_8_c32tomb);

static_assert(MB_LEN_MAX >= 4, "MB_LEN_MAX too small");

const struct lc_ctype __ctype_utf_8 = {
    .codeset = "UTF-8",
    .mbtoc32 = utf_8_mbtoc32,
    .mbstoc32s = utf_8_mbstoc32s,
    .c32tomb_max = 4,
    .c32tomb = utf_8_c32tomb,
    .c32stombs = utf_8_c32stombs,
};
