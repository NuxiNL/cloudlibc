// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <program.h>
#include <stdlib.h>
#include <stdnoreturn.h>

int main(int, char **, char **);

noreturn void program_main(const void *arg, size_t arglen) {
  // In case the program does not provide program_main(), attempt to
  // call into the C standard main() function. Provide at least argv[0]
  // to prevent existing (broken) applications from crashing.
  char program[] = "program";
  char *argv[2] = {program, NULL};
  char *env = NULL;
  exit(main(1, argv, &env));
}
