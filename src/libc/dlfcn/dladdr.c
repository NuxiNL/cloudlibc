// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dlfcn.h>

int dladdr(const void *addr, Dl_info *info) {
  // TODO(ed): How can we implement this? Statically linked binaries
  // do not have .dynsym. The .symtab section is not loaded in memory.
  return 0;
}
