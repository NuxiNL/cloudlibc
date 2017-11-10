// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>

#ifndef dgettext
#error "dgettext is supposed to be a macro as well"
#endif

char *(dgettext)(const char *domain, const char *msgid) {
  return dgettext(domain, msgid);
}
