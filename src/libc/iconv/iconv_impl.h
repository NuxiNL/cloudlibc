// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef ICONV_ICONV_IMPL_H
#define ICONV_ICONV_IMPL_H

#include <common/locale.h>

#include <stdbool.h>

struct __iconv {
  // Character sets used.
  const struct lc_ctype *from;
  const struct lc_ctype *to;

  // Decoding state.
  mbstate_t from_state;

  // Whether to ignore decoding/encoding errors.
  bool ignore;
};

#endif
