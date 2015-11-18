// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libintl.h>

#ifndef dgettext
#error "dgettext is supposed to be a macro as well"
#endif

char *(dgettext)(const char *domain, const char *msgid) {
  return dgettext(domain, msgid);
}
