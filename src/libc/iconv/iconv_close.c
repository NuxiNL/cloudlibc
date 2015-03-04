// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <iconv.h>
#include <stdlib.h>

#include "iconv_impl.h"

int iconv_close(iconv_t cd) {
  free(cd);
  return 0;
}
