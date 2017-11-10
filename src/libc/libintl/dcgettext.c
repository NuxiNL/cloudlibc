// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>

#ifndef dcgettext
#error "dcgettext is supposed to be a macro as well"
#endif

char *(dcgettext)(const char *domain, const char *msgid, int category) {
  return dcgettext(domain, msgid, category);
}
