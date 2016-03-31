// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

struct tls_index {
  unsigned long module;
  unsigned long offset;
};

void *__tls_get_addr(const struct tls_index *);

void *__tls_get_addr(const struct tls_index *index) {
  abort();
  char **tls_base;
#if defined(__aarch64__)
#error "Not implemented"
#elif defined(__x86_64__)
  asm volatile("movq %%fs:0, %0" : "=r"(tls_base));
#else
#error "Not implemented"
#endif
  return tls_base[index->module] + index->offset;
}
