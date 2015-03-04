// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <netdb.h>

const char *gai_strerror(int ecode) {
  return gai_strerror_l(ecode, LC_GLOBAL_LOCALE);
}
