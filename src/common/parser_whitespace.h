// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

// Parser of leading whitespace, used by strto*(), scanf(), etc.

#if WIDE
while (iswspace(PEEK(0)))
  SKIP(1);
#else
{
  const struct lc_ctype *ctype = locale->ctype;
  mbstate_t mbs;
  mbstate_set_init(&mbs);
  for (;;) {
    // Parse bytes until the next wide character is complete.
    size_t peeklen = 0;
    char32_t c32;
    ssize_t ret;
    do {
      char b = PEEK(peeklen);
      ++peeklen;
      ret = ctype->mbtoc32(&c32, &b, 1, &mbs, ctype->data);
    } while (ret == -2);

    // Stop processing if it's not whitespace.
    if (ret <= 0 || !iswspace(c32))
      break;
    SKIP(peeklen);
  }
}
#endif
