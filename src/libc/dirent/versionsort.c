// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>
#include <string.h>

int versionsort(const struct dirent **d1, const struct dirent **d2) {
  return strverscmp((*d1)->d_name, (*d2)->d_name);
}
