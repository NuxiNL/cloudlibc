// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

int cap_getmode(unsigned int *modep) {
  // We're always in Capsicum mode.
  *modep = 1;
  return 0;
}
