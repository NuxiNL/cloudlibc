// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef WCTYPE_WCTYPE_IMPL_H
#define WCTYPE_WCTYPE_IMPL_H

#include <common/locale.h>

#include <sys/types.h>

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <wctype.h>

// The extents table is encoded in such a way that every 32 bit entry
// contains an extent that describes a region of characters in a given
// class. The first 21 bits denote the starting codepoint, while the
// last 11 bits are used to encode the length of the extent.
//
// First do a binary search over the table to find the entry with the
// largest starting codepoint that does not exceed wc. To make the
// comparison work, set the bottom 11 bits in the key. Finally, check
// whether the codepoint is actually in the extent returned.
static inline ssize_t wcbsearch(uint32_t ch, const uint32_t *extents,
                                size_t nextents) {
  uint32_t key = (ch << 11) | 0x7ff;
  size_t min = 0;
  size_t max = nextents - 1;
  do {
    size_t mid = (min + max + 1) / 2;
    if (key < extents[mid])
      max = mid - 1;
    else
      min = mid;
  } while (min < max);
  return ch >= (extents[min] >> 11) &&
                 ch <= (extents[min] >> 11) + (extents[min] & 0x7ff)
             ? (ssize_t)min
             : -1;
}

static inline int iswctype_extents(wint_t wc, const uint32_t *extents,
                                   size_t nextents) {
  return wcbsearch(wc, extents, nextents) >= 0;
}

static inline wint_t towctrans_extents(wint_t wc, const uint32_t *extents,
                                       const int32_t *deltas, size_t nextents) {
  ssize_t idx = wcbsearch(wc, extents, nextents);
  if (idx >= 0)
    return wc + deltas[idx];
  return wc;
}

// For isw*_l() and tow*_l() we should take the locale into account.
// When checking the character type, we should only return a non-zero
// value if the character can be expressed in the character set. When
// transliterating a character, ensure that the target is part of the
// character set.
static inline bool valid_in_locale(wint_t wc, locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  char buf[MB_LEN_MAX];
  return ctype->c32tomb(buf, wc, ctype->data) != -1;
}

#endif
