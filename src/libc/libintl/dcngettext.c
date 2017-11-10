// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>

#ifndef dcngettext
#error "dcngettext is supposed to be a macro as well"
#endif

char *(dcngettext)(const char *domain, const char *msgid1, const char *msgid2,
                   unsigned long n, int category) {
  return dcngettext(domain, msgid1, msgid2, n, category);
}
