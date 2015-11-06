// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <wchar.h>

wint_t putwc(wchar_t wc, FILE *stream) {
  flockfile_orientation(stream, 1);
  wint_t result = putwc_unlocked(wc, stream);
  funlockfile(stream);
  return result;
}

__strong_reference(putwc, fputwc);
