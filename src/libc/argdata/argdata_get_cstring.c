// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

int argdata_get_cstring(const argdata_t *ad, const char **value) {
  // Fetch string value.
  const char *str;
  size_t len;
  int error = argdata_get_string(ad, &str, &len);
  if (error != 0)
    return error;

  // Ensure that it is a C string.
  if (strlen(str) != len)
    return EILSEQ;
  *value = str;
  return 0;
}
