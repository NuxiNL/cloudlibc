// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdarg.h>
#include <syslog.h>

void vsyslog(int priority, const char *message, va_list ap) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_MESSAGES_MASK | LC_NUMERIC_MASK);
  vsyslog_l(priority, locale, message, ap);
}
