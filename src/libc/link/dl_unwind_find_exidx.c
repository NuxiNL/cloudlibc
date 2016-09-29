// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

#include <link.h>
#include <stddef.h>

#ifdef __arm__

void *dl_unwind_find_exidx(const void *pc, int *pcount) {
  // Extract exception information from PT_ARM_EXIDX.
  // TODO(ed): Why does this need to be a libc function? Can't libunwind
  // just use dl_iterate_phdr() like it does on other architectures?
  for (ElfW(Half) i = 0; i < __at_phnum; ++i) {
    const ElfW(Phdr) *phdr = &__at_phdr[i];
    if (phdr->p_type == PT_ARM_EXIDX) {
      *pcount = phdr->p_memsz / (2 * sizeof(void *));
      return (char *)__at_base + phdr->p_vaddr;
    }
  }
  *pcount = 0;
  return NULL;
}

#endif
