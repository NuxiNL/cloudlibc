// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/types.h>

#include <stdbool.h>

#if WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

// Crochemore and Perrin's Two-Way String-Matching.
//
// The Two-Way string matching algorithm works by first computing the
// period and the maximal suffix of the needle, per the critical
// factorization theorem. It then uses these two values to find the
// first matching substring in O(n+m) time, using constant memory.
//
// http://monge.univ-mlv.fr/~mac/Articles-PDF/CP-1991-jacm.pdf
//
// TODO(ed): Should we add a shift table?

#define GENERATE_MAXIMAL_SUFFIX(N, OP)                                   \
  static void maximal_suffix_##N(const char_t *needle, size_t needlelen, \
                                 size_t *suffixpos, size_t *period) {    \
    *suffixpos = 0;                                                      \
    *period = 1;                                                         \
    size_t j = 1;                                                        \
    size_t k = 0;                                                        \
    while (j + k < needlelen) {                                          \
      char_t ap = needle[*suffixpos + k];                                \
      char_t a = needle[j + k];                                          \
      if (a OP ap) {                                                     \
        j += k + 1;                                                      \
        k = 0;                                                           \
        *period = j - *suffixpos;                                        \
      } else if (a == ap) {                                              \
        if (k == *period) {                                              \
          j += *period;                                                  \
          k = 0;                                                         \
        } else {                                                         \
          ++k;                                                           \
        }                                                                \
      } else {                                                           \
        *suffixpos = j++;                                                \
        k = 0;                                                           \
        *period = 1;                                                     \
      }                                                                  \
    }                                                                    \
  }

GENERATE_MAXIMAL_SUFFIX(1, <);
GENERATE_MAXIMAL_SUFFIX(2, >);

#undef GENERATE_MAXIMAL_SUFFIX

static void maximal_suffix(const char_t *needle, size_t needlelen,
                           size_t *suffixpos, size_t *period) {
  size_t suffixpos1, period1;
  maximal_suffix_1(needle, needlelen, &suffixpos1, &period1);
  size_t suffixpos2, period2;
  maximal_suffix_2(needle, needlelen, &suffixpos2, &period2);
  if (suffixpos1 >= suffixpos2) {
    *suffixpos = suffixpos1;
    *period = period1;
  } else {
    *suffixpos = suffixpos2;
    *period = period2;
  }
}

#if !MEMMEM
static bool check_spaceleft(const char_t *haystack, size_t needlelen,
                            const char_t **haystackend) {
  while (*haystackend < haystack + needlelen) {
    if (**haystackend == '\0')
      return false;
    ++*haystackend;
  }
  return true;
}
#endif

#if MEMMEM
static const char_t *twoway_memmem(const char_t *haystack, size_t haystacklen,
                                   const char_t *needle, size_t needlelen) {
  const char_t *haystackend = haystack + haystacklen - needlelen;
#define HAYSTACK_SPACELEFT haystack <= haystackend
#else
static const char_t *twoway_strstr(const char_t *haystack, const char_t *needle,
                                   size_t needlelen) {
  const char_t *haystackend = haystack;
#define HAYSTACK_SPACELEFT check_spaceleft(haystack, needlelen, &haystackend)
#endif
  size_t suffixpos, period;
  maximal_suffix(needle, needlelen, &suffixpos, &period);

#define TWOWAY_PART(MISMATCH)                                 \
  do {                                                        \
    size_t skip = 0;                                          \
    while (HAYSTACK_SPACELEFT) {                              \
      /* Compare the suffix of the needle. */                 \
      size_t i = suffixpos >= skip ? suffixpos : skip;        \
      while (i < needlelen && needle[i] == haystack[i])       \
        ++i;                                                  \
      if (i < needlelen) {                                    \
        /* Mismatch in the suffix. */                         \
        if (skip <= period || i - suffixpos >= skip - period) \
          haystack += i - suffixpos + 1;                      \
        else                                                  \
          haystack += skip - period + 1;                      \
        skip = 0;                                             \
      } else {                                                \
        /* Suffix matched. Compare the prefix as well. */     \
        size_t j = suffixpos;                                 \
        while (j > skip && needle[j - 1] == haystack[j - 1])  \
          --j;                                                \
        /* Complete match. */                                 \
        if (j <= skip)                                        \
          return haystack;                                    \
                                                              \
        /* Mismatch in the prefix. */                         \
        MISMATCH;                                             \
      }                                                       \
    }                                                         \
  } while (0)

#if WIDE
  if (wmemcmp(needle, needle + period, suffixpos) == 0) {
#else
  if (memcmp(needle, needle + period, suffixpos) == 0) {
#endif
    // The needle is completely periodic. This means that in case of a
    // mismatch, we can only advance by the period. Still, we can
    // prevent unnecessary work by skipping data that we've already
    // compared.
    TWOWAY_PART({
      haystack += period;
      skip = needlelen - period;
    });
  } else {
    // The needle is not completely periodic. There is no need to keep
    // track of how much data we need to skip.
    TWOWAY_PART({
      if (suffixpos > needlelen - suffixpos)
        haystack += suffixpos + 1;
      else
        haystack += needlelen - suffixpos + 1;
    });
  }
#undef HAYSTACK_SPACELEFT
#undef TWOWAY_PART
  return NULL;
}
