// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

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
#elif defined(__arm__)
    .machine = "arm\0",
#elif defined(__i386__)
    .machine = "i386\0",
#elif defined(__x86_64__)
    .machine = "x86_64\0",
#else
#error "Unknown architecture"
#endif
  };
  return 0;
}
