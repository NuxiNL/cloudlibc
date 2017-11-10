// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <iconv.h>
#include <stdlib.h>

#include "iconv_impl.h"

int iconv_close(iconv_t cd) {
  free(cd);
  return 0;
}
