// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdarg.h>
#include <syslog.h>

void vsyslog(int priority, const char *message, va_list ap) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_MESSAGES_MASK | LC_NUMERIC_MASK);
  vsyslog_l(priority, locale, message, ap);
}
