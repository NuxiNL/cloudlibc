// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_LOCALE_H
#define COMMON_LOCALE_H

#include <common/refcount.h>

#include <sys/types.h>

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <uchar.h>
#include <wchar.h>

// LC_COLLATE.

struct lconv;

struct lc_collate {};

extern const struct lc_collate __collate_posix;

// LC_CTYPE.

struct lc_ctype {
  // Name of codeset, e.g. "US-ASCII", "UTF-8".
  const char *codeset;

  // Data specific to this codeset. Used by character sets like
  // ISO-8859-x, so they can have different per-dialect tables. Passed
  // to the functions below.
  const void *data;

  // Multibyte to UCS-4 character conversion.
  ssize_t (*mbtoc32)(char32_t *restrict, const char *restrict, size_t,
                     mbstate_t *restrict, const void *restrict);
  ssize_t (*mbstoc32s)(char32_t *restrict, size_t, const char **restrict,
                       size_t, mbstate_t *restrict, const void *restrict, bool);

  // UCS-4 character to multibyte conversion.
  size_t c32tomb_max;
  ssize_t (*c32tomb)(char *restrict, char32_t, const void *restrict);
  ssize_t (*c32stombs)(char *restrict, size_t, const char32_t **restrict,
                       size_t, const void *restrict);
};

// Generates a mbsnrtowcs() function based on mbrtowc(). This allows us
// to implement mbsnrtowcs() without having a single function call per
// character.
#define GENERATE_MBSTOC32S(mbstoc32s, mbtoc32)                         \
  ssize_t mbstoc32s(char32_t *restrict dst, size_t len,                \
                    const char **restrict src, size_t nmc,             \
                    mbstate_t *restrict ps, const void *restrict data, \
                    bool allow_null) {                                 \
    const char *sb = *src;                                             \
    if (dst == NULL) {                                                 \
      /* No output buffer. Compute the length. */                      \
      size_t ret = 0;                                                  \
      for (;;) {                                                       \
        char32_t c32;                                                  \
        ssize_t l = mbtoc32(&c32, sb, nmc, ps, data);                  \
        switch (l) {                                                   \
          case -1:                                                     \
            /* An error occurred. */                                   \
            return -1;                                                 \
          case -2:                                                     \
            /* End of buffer. */                                       \
            return ret;                                                \
          default:                                                     \
            /* Valid character. */                                     \
            if (c32 == U'\0' && !allow_null)                           \
              return ret;                                              \
            sb += l;                                                   \
            nmc -= l;                                                  \
            ++ret;                                                     \
            break;                                                     \
        }                                                              \
      }                                                                \
    } else {                                                           \
      /* Output buffer available. Store results. */                    \
      char32_t *db = dst;                                              \
      while (len-- > 0) {                                              \
        ssize_t l = mbtoc32(db, sb, nmc, ps, data);                    \
        switch (l) {                                                   \
          case -1:                                                     \
            /* An error occurred. */                                   \
            *src = sb;                                                 \
            return -1;                                                 \
          case -2:                                                     \
            /* End of buffer. */                                       \
            *src = sb + nmc;                                           \
            return db - dst;                                           \
          default:                                                     \
            /* Valid character. */                                     \
            if (*db == U'\0' && !allow_null) {                         \
              *src = NULL;                                             \
              return db - dst;                                         \
            }                                                          \
            sb += l;                                                   \
            nmc -= l;                                                  \
            ++db;                                                      \
            break;                                                     \
        }                                                              \
      }                                                                \
      /* Filled entire output buffer. */                               \
      *src = sb;                                                       \
      return db - dst;                                                 \
    }                                                                  \
  }

// Generates a wcsrtombs() function based on wcrtomb().
#define GENERATE_C32STOMBS(c32stombs, c32tomb)                  \
  ssize_t c32stombs(char *restrict dst, size_t len,             \
                    const char32_t **restrict src, size_t nc32, \
                    const void *restrict data) {                \
    if (dst == NULL) {                                          \
      /* No output buffer. Compute the length. */               \
      const char32_t *sb = *src;                                \
      size_t ret = 0;                                           \
      while (nc32-- > 0) {                                      \
        char buf[MB_LEN_MAX];                                   \
        ssize_t l = c32tomb(buf, *sb, data);                    \
        if (l < 0)                                              \
          return -1;                                            \
        ret += l;                                               \
        /* Exclude the null byte from the length. */            \
        if (*sb++ == U'\0')                                     \
          return ret - 1;                                       \
      }                                                         \
      return ret;                                               \
    } else {                                                    \
      /* Output buffer available. Store results. */             \
      char *db = dst;                                           \
      while (nc32-- > 0 && len > 0) {                           \
        char buf[MB_LEN_MAX];                                   \
        ssize_t l = c32tomb(buf, **src, data);                  \
        if (l < 0)                                              \
          return -1;                                            \
        /* Character does not fit in output buffer. */          \
        if ((size_t)l > len)                                    \
          return db - dst;                                      \
        for (size_t i = 0; i < (size_t)l; ++i)                  \
          db[i] = buf[i];                                       \
        if (*(*src)++ == U'\0') {                               \
          *src = NULL;                                          \
          return db - dst;                                      \
        }                                                       \
        db += l;                                                \
        len -= l;                                               \
      }                                                         \
      return db - dst;                                          \
    }                                                           \
  }

// Character sets.
extern const struct lc_ctype __ctype_us_ascii;
extern const struct lc_ctype __ctype_utf_8;

const struct lc_ctype *__lookup_ctype(const char *, size_t);

// LC_MESSAGES.

struct lc_messages {
  const char *yesexpr;  // Affirmative response expression.
  const char *noexpr;   // Negative response expression.

  // TODO(ed): Pick right sizes.
  const char *gai_strerror[11];     // gai_strerror().
  const char *regerror[16];         // regerror().
  const char *strerror[ELAST + 1];  // strerror().
  const char *strsignal[NSIG];      // strsignal().
  const char *unknown_error;        // gai_strerror(), regerror(), strerror().
};

extern const struct lc_messages __messages_en_us;

// LC_MONETARY.

struct lc_monetary {
  const wchar_t *mon_decimal_point;  // Radix character.
  const wchar_t *mon_thousands_sep;  // Separator for groups of digits.
  const signed char *mon_grouping;   // Grouping size.
  const wchar_t *positive_sign;      // Positive sign.
  const wchar_t *negative_sign;      // Negative sign.
  const wchar_t *currency_symbol;    // Local: Currency symbol.
  char frac_digits;                  // Local: Number of fractional digits.
  char p_cs_precedes;                // Local, >= 0: Currency symbol precedes.
  char p_sep_by_space;               // Local, >= 0: Currency symbol separation.
  char p_sign_posn;                  // Local, >= 0: Positive sign position.
  char n_cs_precedes;                // Local, < 0: Currency symbol precedes.
  char n_sep_by_space;               // Local, < 0: Currency symbol separation.
  char n_sign_posn;                  // Local, < 0: Negative sign position.
  char int_curr_symbol[5];           // Int.: Currency symbol.
  char int_frac_digits;              // Int.: Number of fractional digits.
  char int_p_cs_precedes;            // Int., >= 0: Currency symbol precedes.
  char int_p_sep_by_space;           // Int., >= 0: Currency symbol sep.
  char int_p_sign_posn;              // Int., >= 0: Positive sign pos.
  char int_n_cs_precedes;            // Int., < 0: Currency symbol prec.
  char int_n_sep_by_space;           // Int., < 0: Currency symbol sep.
  char int_n_sign_posn;              // Int., < 0: Negative sign pos.
};

extern const struct lc_monetary __monetary_posix;

// LC_NUMERIC.

struct lc_numeric {
  const wchar_t *decimal_point;  // Radix character.
  const wchar_t *thousands_sep;  // Separator for groups of digits.
  const signed char *grouping;   // Grouping size.
};

extern const struct lc_numeric __numeric_posix;

// LC_TIME.

struct lc_time {
  const wchar_t *d_t_fmt;      // String for formatting date and time.
  const wchar_t *d_fmt;        // Date format string.
  const wchar_t *t_fmt;        // Time format string.
  const wchar_t *t_fmt_ampm;   // a.m. or p.m. time format string.
  const wchar_t *am_str;       // Ante-meridiem affix.
  const wchar_t *pm_str;       // Post-meridiem affix.
  const wchar_t *day[7];       // Names of the days of the week.
  const wchar_t *abday[7];     // Abbreviated names of the days of the week.
  const wchar_t *mon[12];      // Names of the months of the year.
  const wchar_t *abmon[12];    // Abbreviated names of the months of the year.
  const wchar_t *era;          // Optional: Era description segments.
  const wchar_t *era_d_fmt;    // Optional: Era date format string.
  const wchar_t *era_d_t_fmt;  // Optional: Era date and time format string.
  const wchar_t *era_t_fmt;    // Optional: Era time format string.
  const wchar_t *alt_digits;   // Optional: Alternative symbols for digits.
};

extern const struct lc_time __time_posix;

// LC_TIMEZONE.

struct lc_timezone_rule {
  uint8_t year_from;      // Lowest year to which this rule applies.
  uint8_t year_to;        // Highest year to which this rule applies.
  uint32_t month : 4;     // Month at which this rule starts.
  uint32_t weekday : 3;   // Weekday at which this rule start (7=dontcare).
  uint32_t monthday : 5;  // Day of the month at which this rule starts.
  uint32_t minute : 11;   // Minutes since 0:00 at which this rule starts.
  uint32_t timebase : 2;  // Which time should be compared against.
#define TIMEBASE_CUR 0    // Rule applies to the current offset.
#define TIMEBASE_STD 1    // Rule applies to the standard time.
#define TIMEBASE_UTC 2    // Rule applies to time in UTC.
  uint32_t save : 4;      // The amount of time in 10 minutes.
  char abbreviation[6];   // Abbreviation of timezone name (e.g., CEST).
};

struct lc_timezone_era {
  const struct lc_timezone_rule *rules;  // Rules associated with this era.
  uint64_t rules_count : 8;              // Number of rules.
  int64_t gmtoff : 18;                   // Offset in seconds relative to GMT.
  int64_t end : 38;                      // Timestamp at which this era ends.
  uint8_t end_save : 4;                  // Daylight savings at the end time.
  char abbreviation_std[6];  // Abbreviation of standard time (e.g., CET).
  char abbreviation_dst[6];  // Abbreviation of DST (e.g., CEST).
};

struct lc_timezone {
  const struct lc_timezone_era *eras;
  size_t eras_count;
};

extern const struct lc_timezone __timezone_utc;

// Compiled string pool.
//
// Functions like strerror(), strsignal(), localeconv() and
// nl_langinfo() are supposed to return references to strings. In this
// implementation, we want to ensure that these strings remain valid
// until the locale the strings were extracted from is deallocated.
//
// It is not possible to return compile-time strings directly, as the
// encoding of these strings should depend on LC_CTYPE. Compile-time
// strings are either stored using wide characters or are encoded using
// UTF-8. They need to be translated before we can return them.
//
// The functions that return these strings are unlikely to be called in
// the common case. As this structure is rather large, it would make
// little sense to allocate space for it unconditionally. To keep the
// size of locale_t small, this structure is only allocated on-demand.
//
// We should use the compiled string pool as little as possible; only
// inside of functions that have to return references to strings.
// Functions like strerror_r() and regerror() should not use this.
//
// This structure may only contain objects having the same layout as
// _Atomic(void *). freelocale() will simply iterate over all of its
// members and call free().

#define DECLARE_STRINGARRAY(category, string) \
  _Atomic(char *)                             \
      category##_##string[__arraycount(((struct lc_##category *)0)->string)]
struct lc_compiled {
  // Compiled strings for LC_MESSAGES.
  _Atomic(char *) messages_yesexpr;
  _Atomic(char *) messages_noexpr;
  DECLARE_STRINGARRAY(messages, gai_strerror);
  DECLARE_STRINGARRAY(messages, strerror);
  DECLARE_STRINGARRAY(messages, strsignal);
  _Atomic(char *) messages_unknown_error;

  // Compiled strings for LC_MONETARY.
  _Atomic(char *) monetary_mon_decimal_point;
  _Atomic(char *) monetary_mon_thousands_sep;
  _Atomic(char *) monetary_positive_sign;
  _Atomic(char *) monetary_negative_sign;
  _Atomic(char *) monetary_currency_symbol;

  // Compiled strings for LC_NUMERIC.
  _Atomic(char *) numeric_decimal_point;
  _Atomic(char *) numeric_thousands_sep;

  // Compiled strings for LC_TIME.
  _Atomic(char *) time_d_t_fmt;
  _Atomic(char *) time_d_fmt;
  _Atomic(char *) time_t_fmt;
  _Atomic(char *) time_t_fmt_ampm;
  _Atomic(char *) time_am_str;
  _Atomic(char *) time_pm_str;
  DECLARE_STRINGARRAY(time, day);
  DECLARE_STRINGARRAY(time, abday);
  DECLARE_STRINGARRAY(time, mon);
  DECLARE_STRINGARRAY(time, abmon);
  _Atomic(char *) time_era;
  _Atomic(char *) time_era_d_fmt;
  _Atomic(char *) time_era_d_t_fmt;
  _Atomic(char *) time_era_t_fmt;
  _Atomic(char *) time_alt_digits;

  // nl_langinfo()'s CRNCYSTR.
  _Atomic(char *) crncystr;

  // Structure returned by localeconv().
  _Atomic(struct lconv *) localeconv;
};
#undef DECLARE_STRINGARRAY

// Obtains strings from the locale in the appropriate character set.
struct lc_compiled *__locale_get_compiled(locale_t);
const char *__locale_compile_string(locale_t, const char *, size_t);
const char *__locale_compile_wstring(locale_t, const wchar_t *, size_t);
#define COMPILE_STRING(locale, category, string)            \
  __locale_compile_string(locale, locale->category->string, \
                          offsetof(struct lc_compiled, category##_##string))
#define COMPILE_WSTRING(locale, category, string)            \
  __locale_compile_wstring(locale, locale->category->string, \
                           offsetof(struct lc_compiled, category##_##string))

// Obtains strings without storing them in the compiled string pool.
size_t __locale_translate_string(locale_t, char *, const char *, size_t);

// Locale object.
struct __locale {
  // Reference count used by newlocale(), duplocale() and freelocale().
  refcount_t refcount;

  // Strings that are character set dependent and are therefore computed
  // on-demand.
  _Atomic(struct lc_compiled *) compiled;

  // Compile-time constant data for individual locale categories.
  const struct lc_collate *collate;
  const struct lc_ctype *ctype;
  const struct lc_messages *messages;
  const struct lc_monetary *monetary;
  const struct lc_numeric *numeric;
  const struct lc_time *time;
  const struct lc_timezone *timezone;
};

// Constructs a temporary locale object that only inherits certain
// classes from the C locale. This macro is typically used by non-_l()
// suffixed functions. By not using LC_GLOBAL_LOCALE, we can prevent
// large structures (like __messages_posix and __collate_posix) from
// being linked in by default.
#if 1
#define DEFAULT_LOCALE(name, mask)                                           \
  static const struct __locale __obj_##name = {                              \
      .compiled = ATOMIC_VAR_INIT((struct lc_compiled *)1),                  \
      .collate = ((mask)&LC_COLLATE_MASK) != 0 ? &__collate_posix : NULL,    \
      .ctype = ((mask)&LC_CTYPE_MASK) != 0 ? &__ctype_us_ascii : NULL,       \
      .messages = ((mask)&LC_MESSAGES_MASK) != 0 ? &__messages_en_us : NULL, \
      .monetary = ((mask)&LC_MONETARY_MASK) != 0 ? &__monetary_posix : NULL, \
      .numeric = ((mask)&LC_NUMERIC_MASK) != 0 ? &__numeric_posix : NULL,    \
      .time = ((mask)&LC_TIME_MASK) != 0 ? &__time_posix : NULL,             \
      .timezone = ((mask)&LC_TIMEZONE_MASK) != 0 ? &__timezone_utc : NULL,   \
  };                                                                         \
  const locale_t name = (struct __locale *)&__obj_##name
#else
#define DEFAULT_LOCALE(name, mask) const locale_t name = LC_GLOBAL_LOCALE
#endif

#endif
