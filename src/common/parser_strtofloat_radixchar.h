// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

// Parser for the radix character, used by parser_strtofloat.h.

bool parsed_radixchar = false;

{
  // In a locale where the radix character is not defined, the radix
  // character shall default to a '.'.
  const wchar_t *decimal_point = locale->numeric->decimal_point;
  if (decimal_point == NULL || *decimal_point == L'\0')
    decimal_point = L".";

  size_t peeklen = 0;
  for (;;) {
    // TODO(ed): mbtowc() if !WIDE.
    if (*decimal_point == L'\0') {
      // Full match on the radix character.
      SKIP(peeklen);
      parsed_radixchar = true;
      break;
    } else if (*decimal_point == PEEK(peeklen)) {
      // Character matched. Compare the next.
      ++peeklen;
      ++decimal_point;
    } else {
      // Mismatch.
      break;
    }
  }
}
