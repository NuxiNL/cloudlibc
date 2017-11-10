// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>

#include <link.h>
#include <stddef.h>

int dl_iterate_phdr(int (*callback)(struct dl_phdr_info *, size_t, void *),
                    void *data) {
  // This environment does not support any dynamic linking. Invoke the
  // callback once, providing it information about just the executable.
  struct dl_phdr_info info = {
      .dlpi_addr = (uintptr_t)__at_base,
      .dlpi_name = "unknown",
      .dlpi_phdr = __at_phdr,
      .dlpi_phnum = __at_phnum,
      .dlpi_adds = 1,
      .dlpi_subs = 0,
      .dlpi_tls_modid = 1,
      .dlpi_tls_data = (void *)__pt_tls_vaddr_abs,
  };
  return callback(&info, sizeof(info), data);
}
