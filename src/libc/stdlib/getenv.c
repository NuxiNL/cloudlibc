// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

char *getenv(const char *name) {
  // Environment variables are not available in this environment.
  return NULL;
}
