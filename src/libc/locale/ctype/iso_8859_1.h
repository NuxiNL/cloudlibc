// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>

static ssize_t iso_8859_1_mbtoc32(char32_t *restrict pc32,
                                  const char *restrict s, size_t n,
                                  mbstate_t *restrict ps,
                                  const void *restrict data) {
  // Return next byte directly. No translation is needed, as ISO 8859-1
  // maps directly onto Unicode.
  if (n < 1)
    return -2;
  *pc32 = (unsigned char)*s;
  mbstate_set_init(ps);
  return 1;
}

static GENERATE_MBSTOC32S(iso_8859_1_mbstoc32s, iso_8859_1_mbtoc32);

static ssize_t iso_8859_1_c32tomb(char *restrict s, char32_t c32,
                                  const void *restrict data) {
  // Convert character to ISO 8859-1 if within range.
  if (c32 > UCHAR_MAX) {
    errno = EILSEQ;
    return -1;
  }
  *s = c32;
  return 1;
}

static GENERATE_C32STOMBS(iso_8859_1_c32stombs, iso_8859_1_c32tomb);

static_assert(MB_LEN_MAX >= 1, "MB_LEN_MAX too small");

static const struct lc_ctype ctype_iso_8859_1 = {
    .codeset = "ISO-8859-1",
    .mbtoc32 = iso_8859_1_mbtoc32,
    .mbstoc32s = iso_8859_1_mbstoc32s,
    .c32tomb_max = 1,
    .c32tomb = iso_8859_1_c32tomb,
    .c32stombs = iso_8859_1_c32stombs,
};
