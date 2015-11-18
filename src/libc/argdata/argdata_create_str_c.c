// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <string.h>

argdata_t *argdata_create_str_c(const char *value) {
  return argdata_create_str(value, strlen(value));
}
