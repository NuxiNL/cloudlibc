// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <testing.h>

int main(int argc, char **argv) {
  fswap(stderr, fdopen(1, "w"));
  testing_execute(0, 1);
  return 0;
}
