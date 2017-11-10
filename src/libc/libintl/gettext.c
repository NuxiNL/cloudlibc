// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>

#ifndef gettext
#error "gettext is supposed to be a macro as well"
#endif

char *(gettext)(const char *msgid) {
  return gettext(msgid);
}
