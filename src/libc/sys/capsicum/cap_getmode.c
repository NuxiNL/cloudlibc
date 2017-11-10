// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

int cap_getmode(unsigned int *modep) {
  // We're always in Capsicum mode.
  *modep = 1;
  return 0;
}
