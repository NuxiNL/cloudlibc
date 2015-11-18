// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

int argdata_get_str_c(const argdata_t *ad, const char **value) {
  // Fetch string value.
  const char *str;
  size_t len;
  int error = argdata_get_str(ad, &str, &len);
  if (error != 0)
    return error;

  // Ensure that it is a C string.
  if (strlen(str) != len)
    return EILSEQ;
  *value = str;
  return 0;
}
