// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <locale.h> - category macros
//
// Extensions:
// - LC_TIMEZONE_MASK:
//   Handle to timezone. Used by localtime_l() and mktime_l().
// - LC_C_LOCALE:
//   Explicit handle to the standard C locale. Also present on NetBSD.
// - LC_C_UNICODE_LOCALE:
//   Identical to LC_C_LOCALE, except that it uses UTF-8 for LC_CTYPE.
// - localeconv_l():
//   localeconv() always uses the global locale.
//
// Features missing:
// - uselocale():
//   This environment does not provide global locales. Use *_l() instead.

#ifndef _LOCALE_H_
#define _LOCALE_H_

#include <_/types.h>

#define NULL _NULL

#ifndef _LOCALE_T_DECLARED
typedef __locale_t locale_t;
#define _LOCALE_T_DECLARED
#endif

// Structure returned by localeconv().
struct lconv {
  char *decimal_point;  // Numeric: Radix character.
  char *thousands_sep;  // Numeric: Separator for groups of digits.
  char *grouping;       // Numeric: Grouping size.

  char *mon_decimal_point;  // Monetary: Radix character.
  char *mon_thousands_sep;  // Monetary: Separator for groups of digits.
  char *mon_grouping;       // Monetary: Grouping size.
  char *positive_sign;      // Monetary: Positive sign.
  char *negative_sign;      // Monetary: Negative sign.
  char *currency_symbol;    // Monetary, local: Currency symbol.
  char frac_digits;         // Monetary, local: Number of fractional digits.
  char p_cs_precedes;       // Monetary, local, >= 0: Currency symbol precedes.
  char p_sep_by_space;     // Monetary, local, >= 0: Currency symbol separation.
  char p_sign_posn;        // Monetary, local, >= 0: Positive sign position.
  char n_cs_precedes;      // Monetary, local, < 0: Currency symbol precedes.
  char n_sep_by_space;     // Monetary, local, < 0: Currency symbol separation.
  char n_sign_posn;        // Monetary, local, < 0: Negative sign position.
  char *int_curr_symbol;   // Monetary, int.: Currency symbol.
  char int_frac_digits;    // Monetary, int.: Number of fractional digits.
  char int_p_cs_precedes;  // Monetary, int., >= 0: Currency symbol precedes.
  char int_p_sep_by_space;  // Monetary, int., >= 0: Currency symbol separation.
  char int_p_sign_posn;     // Monetary, int., >= 0: Positive sign position.
  char int_n_cs_precedes;   // Monetary, int., < 0: Currency symbol precedes.
  char int_n_sep_by_space;  // Monetary, int., < 0: Currency symbol separation.
  char int_n_sign_posn;     // Monetary, int., < 0: Negative sign position.
};

// Categories used by setlocale().
#define LC_ALL (-1)
#define LC_COLLATE 0
#define LC_CTYPE 1
#define LC_MESSAGES 2
#define LC_MONETARY 3
#define LC_NUMERIC 4
#define LC_TIME 5
#define _LC_TIMEZONE 6

// Category bits used by newlocale().
#define LC_COLLATE_MASK (1 << LC_COLLATE)
#define LC_CTYPE_MASK (1 << LC_CTYPE)
#define LC_MESSAGES_MASK (1 << LC_MESSAGES)
#define LC_MONETARY_MASK (1 << LC_MONETARY)
#define LC_NUMERIC_MASK (1 << LC_NUMERIC)
#define LC_TIME_MASK (1 << LC_TIME)
#define LC_TIMEZONE_MASK (1 << _LC_TIMEZONE)
#define LC_ALL_MASK                                                        \
  (LC_COLLATE_MASK | LC_CTYPE_MASK | LC_MESSAGES_MASK | LC_MONETARY_MASK | \
   LC_NUMERIC_MASK | LC_TIME_MASK | LC_TIMEZONE_MASK)

// Short-hand identifiers for commonly used locales.
extern struct __locale __locale_c;
extern struct __locale __locale_c_unicode;
#define LC_C_LOCALE (&__locale_c)
#define LC_C_UNICODE_LOCALE (&__locale_c_unicode)

// The global locale cannot be modified in this implementation, so let
// it point directly to the C locale.
#define LC_GLOBAL_LOCALE LC_C_LOCALE

__BEGIN_DECLS
locale_t duplocale(locale_t);
void freelocale(locale_t);
struct lconv *localeconv(void);
struct lconv *localeconv_l(locale_t);
locale_t newlocale(int, const char *, locale_t);
char *setlocale(int, const char *);
__END_DECLS

#endif
