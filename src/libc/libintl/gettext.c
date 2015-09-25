// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libintl.h>

#ifndef gettext
#error "gettext is supposed to be a macro as well"
#endif

char *(gettext)(const char *msgid) {
  return gettext(msgid);
}
