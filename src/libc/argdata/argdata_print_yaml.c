// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <stdbool.h>
#include <stdio.h>

static int print_yaml(const argdata_t *ad, FILE *fp, unsigned int depth) {
  bool bool_value;
  if (argdata_get_bool(ad, &bool_value) == 0) {
    fputs(bool_value ? "!!bool \"true\"" : "!!bool \"false\"", fp);
  } else {
    fputs("!!null \"null\"", fp);
  }
  return 0;
}

int argdata_print_yaml(const argdata_t *ad, FILE *fp) {
  fputs("---\n", fp);
  int error = print_yaml(ad, fp, 0);
  if (error != 0)
    return error;
  fputc('\n', fp);
  return 0;
}
