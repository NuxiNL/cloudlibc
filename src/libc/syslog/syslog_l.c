// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdarg.h>
#include <syslog.h>

void syslog_l(int priority, locale_t locale, const char *message, ...) {
  va_list ap;
  va_start(ap, message);
  vsyslog_l(priority, locale, message, ap);
  va_end(ap);
}
