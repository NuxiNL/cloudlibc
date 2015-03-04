// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef atof
#error "atof is supposed to be a macro as well"
#endif

// clang-format off
double (atof)(const char *str) {
  return atof(str);
}
