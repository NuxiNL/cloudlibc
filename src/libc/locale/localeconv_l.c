// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

struct lconv *localeconv_l(locale_t locale) {
  struct lc_compiled *compiled = __locale_get_compiled(locale);
  if (compiled == NULL)
    return NULL;

  // Return an already existing copy if available.
  struct lconv *old_lconv = atomic_load(&compiled->localeconv);
  if (old_lconv != NULL)
    return old_lconv;

  // No copy is available, so create it right now.
  struct lconv *new_lconv = malloc(sizeof(*new_lconv));
  if (new_lconv == NULL)
    return NULL;
  *new_lconv = (struct lconv){
#define CHAR(category, field) .field = locale->category->field
#define STRING(category, field)                                              \
  .field = locale->category->field != NULL ? (char *)locale->category->field \
                                           : (char *)""
#define WSTRING(category, field) \
  .field = (char *)COMPILE_WSTRING(locale, category, field)
      // Fields provided by LC_NUMERIC.
      WSTRING(numeric, decimal_point),
      WSTRING(numeric, thousands_sep),
      STRING(numeric, grouping),

      // Fields provided by LC_MONETARY.
      WSTRING(monetary, mon_decimal_point),
      WSTRING(monetary, mon_thousands_sep),
      STRING(monetary, mon_grouping),
      WSTRING(monetary, positive_sign),
      WSTRING(monetary, negative_sign),
      WSTRING(monetary, currency_symbol),
      CHAR(monetary, frac_digits),
      CHAR(monetary, p_cs_precedes),
      CHAR(monetary, p_sep_by_space),
      CHAR(monetary, p_sign_posn),
      CHAR(monetary, n_cs_precedes),
      CHAR(monetary, n_sep_by_space),
      CHAR(monetary, n_sign_posn),
      STRING(monetary, int_curr_symbol),
      CHAR(monetary, int_frac_digits),
      CHAR(monetary, int_p_cs_precedes),
      CHAR(monetary, int_p_sep_by_space),
      CHAR(monetary, int_p_sign_posn),
      CHAR(monetary, int_n_cs_precedes),
      CHAR(monetary, int_n_sep_by_space),
      CHAR(monetary, int_n_sign_posn),
#undef CHAR
#undef STRING
#undef WSTRING
  };

  // Store the new copy and return it.
  if (!atomic_compare_exchange_strong(&compiled->localeconv, &old_lconv,
                                      new_lconv)) {
    // Race condition. Another thread created a copy at the same time.
    free(new_lconv);
    return old_lconv;
  }
  return new_lconv;
}
