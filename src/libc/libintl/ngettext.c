// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libintl.h>

#ifndef ngettext
#error "ngettext is supposed to be a macro as well"
#endif

char *(ngettext)(const char *msgid1, const char *msgid2, unsigned long n) {
  return ngettext(msgid1, msgid2, n);
}
