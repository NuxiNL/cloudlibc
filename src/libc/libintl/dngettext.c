// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>

#ifndef dngettext
#error "dngettext is supposed to be a macro as well"
#endif

char *(dngettext)(const char *domain, const char *msgid1, const char *msgid2,
                  unsigned long n) {
  return dngettext(domain, msgid1, msgid2, n);
}
