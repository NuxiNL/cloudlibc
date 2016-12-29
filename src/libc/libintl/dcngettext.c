// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libintl.h>

#ifndef dcngettext
#error "dcngettext is supposed to be a macro as well"
#endif

char *(dcngettext)(const char *domain, const char *msgid1, const char *msgid2,
                   unsigned long n, int category) {
  return dcngettext(domain, msgid1, msgid2, n, category);
}
