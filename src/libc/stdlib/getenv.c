// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <string.h>

extern char **environ;

char *getenv(const char *name) {
  // Scan the environment for a matching variable and return the value.
  size_t varlen = strlen(name);
  for (char **var = environ; *var != NULL; ++var)
    if (memcmp(*var, name, varlen) == 0 && (*var)[varlen] == '=')
      return (*var) + varlen + 1;
  return NULL;
}
