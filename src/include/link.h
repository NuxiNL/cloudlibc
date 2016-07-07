// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <link.h> - Run-time linker

#ifndef _LINK_H_
#define _LINK_H_

#include <elf.h>

#define ElfW(type) _ElfW(__INTPTR_WIDTH__, type)
#define _ElfW(width, type) __ElfW(width, type)
#define __ElfW(width, type) Elf##width##_##type
#define ELFW(type) _ELFW(__INTPTR_WIDTH__, type)
#define _ELFW(width, type) __ELFW(width, type)
#define __ELFW(width, type) ELF##width##_##type

struct dl_phdr_info {
  ElfW(Addr) dlpi_addr;          // Base address.
  const char *dlpi_name;         // Pathname of the object.
  const ElfW(Phdr) * dlpi_phdr;  // Program headers.
  ElfW(Half) dlpi_phnum;         // Number of program headers.
  unsigned long long dlpi_adds;  // Object loads.
  unsigned long long dlpi_subs;  // Object unloads.
  __size_t dlpi_tls_modid;       // TLS index.
  void *dlpi_tls_data;           // TLS initialization data.
};

__BEGIN_DECLS
int dl_iterate_phdr(int (*)(struct dl_phdr_info *, __size_t, void *), void *);
__END_DECLS

#endif
