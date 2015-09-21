// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>
#include <string.h>

int alphasort(const struct dirent **d1, const struct dirent **d2) {
  return strcoll((*d1)->d_name, (*d2)->d_name);
}
