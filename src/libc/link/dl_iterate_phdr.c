// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

#include <link.h>
#include <stddef.h>

int dl_iterate_phdr(int (*callback)(struct dl_phdr_info *, size_t, void *),
                    void *data) {
  // This environment does not support any dynamic linking. Invoke the
  // callback once, providing it information about just the executable.
  struct dl_phdr_info info = {
      .dlpi_addr = 0x0,
      .dlpi_name = "unknown",
      .dlpi_phdr = __elf_phdr,
      .dlpi_phnum = __elf_phnum,
      .dlpi_adds = 1,
      .dlpi_subs = 0,
      .dlpi_tls_modid = 1,
      .dlpi_tls_data = (void *)__tls_init_data,
  };
  return callback(&info, sizeof(info), data);
}
