// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x

const char *uv_version_string(void) {
  return STRINGIFY(UV_VERSION_MAJOR) "." STRINGIFY(
      UV_VERSION_MINOR) "." STRINGIFY(UV_VERSION_PATCH)
#if !UV_VERSION_IS_RELEASE
      "-" UV_VERSION_SUFFIX
#endif
      ;
}
