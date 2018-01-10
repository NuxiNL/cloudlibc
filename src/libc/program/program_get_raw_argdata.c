// Copyright (c) 2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>

#include <program.h>

void program_get_raw_argdata(const void **buf, size_t *len) {
  *buf = __at_argdata;
  *len = __at_argdatalen;
}
