// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

wchar_t *wcstok(wchar_t *restrict ws, const wchar_t *restrict sep,
                wchar_t **restrict lastws) {
  // Restore saved token.
  if (ws == NULL) {
    ws = *lastws;
    if (ws == NULL)
      return NULL;
  }

  // Skip leading delimiters.
  ws += wcsspn(ws, sep);
  if (*ws == L'\0') {
    *lastws = NULL;
    return NULL;
  }

  // Find end of token.
  wchar_t *end = ws + wcscspn(ws, sep);
  if (*end == L'\0') {
    // Token is at the end of the input.
    *lastws = NULL;
  } else {
    // Token is not at the end of the input, so null terminate it.
    *end = L'\0';
    *lastws = end + 1;
  }
  return ws;
}
