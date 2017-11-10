// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <langinfo.h>
#include <limits.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

char *nl_langinfo_l(nl_item item, locale_t locale) {
  switch (item) {
    case CODESET:
      return (char *)locale->ctype->codeset;
#define STRING(item, category, field) \
  case item:                          \
    return (char *)COMPILE_STRING(locale, category, field)
#define WSTRING(item, category, field) \
  case item:                           \
    return (char *)COMPILE_WSTRING(locale, category, field)
      WSTRING(D_T_FMT, time, d_t_fmt);
      WSTRING(D_FMT, time, d_fmt);
      WSTRING(T_FMT, time, t_fmt);
      WSTRING(T_FMT_AMPM, time, t_fmt_ampm);
      WSTRING(AM_STR, time, am_str);
      WSTRING(PM_STR, time, pm_str);
      WSTRING(DAY_1, time, day[0]);
      WSTRING(DAY_2, time, day[1]);
      WSTRING(DAY_3, time, day[2]);
      WSTRING(DAY_4, time, day[3]);
      WSTRING(DAY_5, time, day[4]);
      WSTRING(DAY_6, time, day[5]);
      WSTRING(DAY_7, time, day[6]);
      WSTRING(ABDAY_1, time, abday[0]);
      WSTRING(ABDAY_2, time, abday[1]);
      WSTRING(ABDAY_3, time, abday[2]);
      WSTRING(ABDAY_4, time, abday[3]);
      WSTRING(ABDAY_5, time, abday[4]);
      WSTRING(ABDAY_6, time, abday[5]);
      WSTRING(ABDAY_7, time, abday[6]);
      WSTRING(MON_1, time, mon[0]);
      WSTRING(MON_2, time, mon[1]);
      WSTRING(MON_3, time, mon[2]);
      WSTRING(MON_4, time, mon[3]);
      WSTRING(MON_5, time, mon[4]);
      WSTRING(MON_6, time, mon[5]);
      WSTRING(MON_7, time, mon[6]);
      WSTRING(MON_8, time, mon[7]);
      WSTRING(MON_9, time, mon[8]);
      WSTRING(MON_10, time, mon[9]);
      WSTRING(MON_11, time, mon[10]);
      WSTRING(MON_12, time, mon[11]);
      WSTRING(ABMON_1, time, abmon[0]);
      WSTRING(ABMON_2, time, abmon[1]);
      WSTRING(ABMON_3, time, abmon[2]);
      WSTRING(ABMON_4, time, abmon[3]);
      WSTRING(ABMON_5, time, abmon[4]);
      WSTRING(ABMON_6, time, abmon[5]);
      WSTRING(ABMON_7, time, abmon[6]);
      WSTRING(ABMON_8, time, abmon[7]);
      WSTRING(ABMON_9, time, abmon[8]);
      WSTRING(ABMON_10, time, abmon[9]);
      WSTRING(ABMON_11, time, abmon[10]);
      WSTRING(ABMON_12, time, abmon[11]);
      WSTRING(ERA, time, era);
      WSTRING(ERA_D_FMT, time, era_d_fmt);
      WSTRING(ERA_D_T_FMT, time, era_d_t_fmt);
      WSTRING(ERA_T_FMT, time, era_t_fmt);
      WSTRING(ALT_DIGITS, time, alt_digits);
      WSTRING(RADIXCHAR, numeric, decimal_point);
      WSTRING(THOUSEP, numeric, thousands_sep);
      STRING(YESEXPR, messages, yesexpr);
      STRING(NOEXPR, messages, noexpr);
#undef STRING
#undef WSTRING
    case CRNCYSTR: {
      // CRNCYSTR should return the currency symbol used by the locale,
      // preceded by a character indicating where the currency symbol
      // should be placed.
      //
      // Instead of storing it separately, we can derive it from several
      // existing properties.

      struct lc_compiled *compiled = __locale_get_compiled(locale);
      if (compiled == NULL)
        return NULL;

      // Return an already existing copy if available.
      char *old_crncystr = atomic_load(&compiled->crncystr);
      if (old_crncystr != NULL)
        return (char *)old_crncystr;

      // String only makes sense if there is a currency symbol and its
      // placement is uniform.
      const struct lc_monetary *monetary = locale->monetary;
      const wchar_t *currency_symbol = monetary->currency_symbol;
      if (currency_symbol == NULL || *currency_symbol == '\0' ||
          monetary->p_cs_precedes != monetary->n_cs_precedes)
        return (char *)"";

      // Determine the character preceding the currency symbol,
      // indicating the position.
      char precedes = monetary->p_cs_precedes;
      char position;
      if (precedes == CHAR_MAX) {
        if (wcscmp(currency_symbol, monetary->mon_decimal_point) != 0)
          return (char *)"";
        position = '.';
      } else {
        position = precedes ? '-' : '+';
      }

      // Generate new string.
      char *new_crncystr;
      if (asprintf_l(&new_crncystr, locale, "%c%ls", position,
                     currency_symbol) == -1)
        return (char *)"";

      // Store the new copy and return it.
      if (!atomic_compare_exchange_strong(&compiled->crncystr, &old_crncystr,
                                          new_crncystr)) {
        // Race condition. Another thread created a copy at the same time.
        free(new_crncystr);
        return old_crncystr;
      }
      return new_crncystr;
    }
    default:
      return (char *)"";
  }
}
