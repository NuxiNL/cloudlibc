// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <langinfo.h>
#include <locale.h>

char *nl_langinfo(nl_item item) {
  return nl_langinfo_l(item, LC_GLOBAL_LOCALE);
}
