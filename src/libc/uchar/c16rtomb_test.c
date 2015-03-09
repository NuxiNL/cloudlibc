// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <testing.h>
#include <uchar.h>

TEST(c16rtomb, ascii) {
  char c;
  mbstate_t mbs = {};

  // Valid characters.
  ASSERT_EQ(1, c16rtomb(&c, u'A', &mbs));
  ASSERT_EQ('A', c);
  ASSERT_EQ(1, c16rtomb(&c, u'\0', &mbs));
  ASSERT_EQ('\0', c);

  // Characters cannot be expressed as ASCII.
  ASSERT_EQ((size_t)-1, c16rtomb(&c, u'€', &mbs));
  ASSERT_EQ(EILSEQ, errno);

  // Surrogate pair. Cannot be expressed as ASCII.
  ASSERT_EQ(0, c16rtomb(&c, 0xd801, &mbs));
  ASSERT_EQ((size_t)-1, c16rtomb(&c, 0xdc37, &mbs));
  ASSERT_EQ(EILSEQ, errno);
}

TEST(c16rtomb, unicode) {
  char buf[MB_LEN_MAX];
  mbstate_t mbs = {};

  // Valid characters.
  ASSERT_EQ(1, c16rtomb_l(buf, u'A', &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ('A', buf[0]);
  ASSERT_EQ(1, c16rtomb_l(buf, u'\0', &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ('\0', buf[0]);
  ASSERT_EQ(3, c16rtomb_l(buf, u'€', &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_ARREQ("€", buf, 3);

  // Surrogate pair.
  ASSERT_EQ(0, c16rtomb_l(buf, 0xd801, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(4, c16rtomb_l(buf, 0xdc37, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_ARREQ("𐐷", buf, 3);
}
