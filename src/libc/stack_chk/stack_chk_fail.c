// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void __stack_chk_fail(void);

noreturn void __stack_chk_fail(void) {
  fprintf(stderr, "Buffer overflow detected\n");
  abort();
}
