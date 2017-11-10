// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>

static ssize_t us_ascii_mbtoc32(char32_t *restrict pc32, const char *restrict s,
                                size_t n, mbstate_t *restrict ps,
                                const void *restrict data) {
  // Return next ASCII byte if it's valid.
  if (n < 1)
    return -2;
  unsigned char b = *s;
  if (b > SCHAR_MAX) {
    errno = EILSEQ;
    return -1;
  }
  *pc32 = b;
  mbstate_set_init(ps);
  return 1;
}

static GENERATE_MBSTOC32S(us_ascii_mbstoc32s, us_ascii_mbtoc32);

static ssize_t us_ascii_c32tomb(char *restrict s, char32_t c32,
                                const void *restrict data) {
  // Convert character to ASCII if within range.
  if (c32 > SCHAR_MAX) {
    errno = EILSEQ;
    return -1;
  }
  *s = c32;
  return 1;
}

static GENERATE_C32STOMBS(us_ascii_c32stombs, us_ascii_c32tomb);

static_assert(MB_LEN_MAX >= 1, "MB_LEN_MAX too small");

const struct lc_ctype __ctype_us_ascii = {
    .codeset = "US-ASCII",
    .mbtoc32 = us_ascii_mbtoc32,
    .mbstoc32s = us_ascii_mbstoc32s,
    .c32tomb_max = 1,
    .c32tomb = us_ascii_c32tomb,
    .c32stombs = us_ascii_c32stombs,
};
