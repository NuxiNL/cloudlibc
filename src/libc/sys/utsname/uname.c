// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/utsname.h>

#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x
#define CLOUDLIBC_VERSION \
  STRINGIFY(__cloudlibc_major__) "." STRINGIFY(__cloudlibc_minor__)

int uname(struct utsname *uname) {
  *uname = (struct utsname) {
    .sysname = "cloudlibc\0", .release = CLOUDLIBC_VERSION "\0",
    .version = CLOUDLIBC_VERSION "\0",
#if defined(__aarch64__)
    .machine = "aarch64\0",
#elif defined(__arm__) && __ARM_ARCH == 6
    .machine = "armv6\0",
#elif defined(__arm__) && __ARM_ARCH == 7
    .machine = "armv7\0",
#elif defined(__i386__)
    .machine = "i386\0",
#elif defined(__riscv) && __riscv_xlen == 32
    .machine = "riscv32\0",
#elif defined(__riscv) && __riscv_xlen == 64
    .machine = "riscv64\0",
#elif defined(__x86_64__)
    .machine = "x86_64\0",
#else
#error "Unknown architecture"
#endif
  };
  return 0;
}
