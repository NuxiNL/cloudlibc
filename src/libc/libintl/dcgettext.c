// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libintl.h>

#ifndef dcgettext
#error "dcgettext is supposed to be a macro as well"
#endif

char *(dcgettext)(const char *domain, const char *msgid, int category) {
  return dcgettext(domain, msgid, category);
}
