// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/tls.h>

#include <dlfcn.h>
#include <stdbool.h>

int dladdr(const void *addr, Dl_info *info) {
  // Determine whether the requested object is placed within this
  // thread's TLS area. We'd want to search for STT_TLS entries in that
  // case. Don't attempt to do lookups for TLS areas of other threads,
  // as that could make the data returned by this function unusable due
  // to races.
  const char *exec_base = __at_base;
  const char *tls_base = tls_get();
  const char *sym_addr = addr;
  const char *sym_base;
  bool is_tls;
  if (sym_addr >= tls_base && sym_addr < tls_base + __pt_tls_memsz_aligned) {
    sym_base = tls_base;
    is_tls = true;
  } else if (sym_addr >= exec_base) {
    sym_base = exec_base;
    is_tls = false;
  } else {
    return 0;
  }
  uintptr_t rel_addr = (const char *)addr - sym_base;

  // Extract the Dynamic Section of the executable.
  const ElfW(Phdr) *phdr = __at_phdr;
  size_t phnum = __at_phnum;
  for (;;) {
    if (phnum-- == 0)
      return 0;
    if (phdr->p_type == PT_DYNAMIC)
      break;
    ++phdr;
  }

  // Extract the symbol and string tables.
  const ElfW(Dyn) *dyn = (const ElfW(Dyn) *)(exec_base + phdr->p_vaddr);
  const char *str = NULL;
  const ElfW(Sym) *sym = NULL;
  size_t symsz = 0;
  while (dyn->d_tag != DT_NULL) {
    switch (dyn->d_tag) {
      case DT_HASH:
        // Number of symbols in the symbol table can only be extracted
        // by fetching the number of chains in the symbol hash table.
        symsz = ((const Elf32_Word *)(exec_base + dyn->d_un.d_ptr))[1];
        break;
      case DT_STRTAB:
        str = exec_base + dyn->d_un.d_ptr;
        break;
      case DT_SYMTAB:
        sym = (const ElfW(Sym) *)(exec_base + dyn->d_un.d_ptr);
        break;
    }
    ++dyn;
  }

  // Scan through the symbol table and find a matching entry. A scan is
  // needed, as entries are not necessarily sorted by address. Prefer
  // returning the entry with the smallest symbol size.
  const ElfW(Sym) *best = NULL;
  while (symsz-- > 0) {
    if (is_tls == (ELFW(ST_TYPE)(sym->st_info) == STT_TLS) &&
        rel_addr >= sym->st_value && rel_addr < sym->st_value + sym->st_size &&
        sym->st_name > 0 && (best == NULL || sym->st_size < best->st_size)) {
      best = sym;
    }
    ++sym;
  }

  if (best == NULL) {
    return 0;
  } else {
    *info = (Dl_info){
        .dli_fname = "unknown",
        .dli_fbase = (void *)exec_base,
        .dli_sname = str + best->st_name,
        .dli_saddr = (void *)(sym_base + best->st_value),
    };
    return 1;
  }
}
