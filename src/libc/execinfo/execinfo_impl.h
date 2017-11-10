// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef EXECINFO_EXECINFO_IMPL_H
#define EXECINFO_EXECINFO_IMPL_H

#include <dlfcn.h>
#include <stddef.h>

// Information of a stackframe, used for printing.
struct frame {
  const void *address;
  const char *symbol;
  ptrdiff_t offset;
};

// Fetches information of a stackframe.
static inline void frame_get(struct frame *f, const void *address) {
  f->address = address;
  Dl_info info;
  if (dladdr(address, &info)) {
    // Symbol name and starting address available.
    *f = (struct frame){
        .address = address,
        .symbol = info.dli_sname,
        .offset = (const char *)address - (const char *)info.dli_saddr,
    };
  } else {
    // No symbol name available. Just print the address.
    *f = (struct frame){
        .address = address,
    };
  }
}

// Prints a stackframe using, using a specified printing routine.
#define FRAME_PRINT(f, printf, suffix, ...)                               \
  ((f)->symbol == NULL                                                    \
       ? printf(__VA_ARGS__, "%p" suffix, (f)->address)                   \
       : (f)->offset == 0 ? printf(__VA_ARGS__, "%p == %s" suffix,        \
                                   (f)->address, (f)->symbol)             \
                          : printf(__VA_ARGS__, "%p == %s + %#tx" suffix, \
                                   (f)->address, (f)->symbol, (f)->offset))

#endif
