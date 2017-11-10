// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata_impl.h>
#include <program.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

int main(int, char **, char **);

// Fallback for when the program does not contain program_main(), so
// that it can call into the C standard main() function.
noreturn void program_main(const argdata_t *ad) {
  // Copy the argument data over to a writable null-terminated buffer.
  char arg[ad->length + 1];
  memcpy(arg, ad->buffer.buffer, ad->length);
  arg[ad->length] = '\0';

  // Count the number of arguments.
  size_t argc = 0;
  for (size_t i = 0; i <= ad->length; ++i)
    if (arg[i] == '\0')
      ++argc;

  // Fill argv array with pointers to individual strings.
  char *argv[argc + 1];
  char *p = arg;
  for (size_t i = 0; i < argc; ++i) {
    argv[i] = p;
    p += strlen(p) + 1;
  }
  argv[argc] = NULL;
  char *env = NULL;
  exit(main(argc, argv, &env));
}
