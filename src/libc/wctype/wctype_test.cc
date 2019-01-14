// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <wctype.h>

#include "gtest/gtest.h"

TEST(wctype, classes) {
  for (wint_t wc = 0; wc <= 0x10ffff; ++wc) {
    SCOPED_TRACE(wc);

    bool alnum = iswalnum(wc);
    bool alpha = iswalpha(wc);
    bool blank = iswblank(wc);
    bool cntrl = iswcntrl(wc);
    bool digit = iswdigit(wc);
    bool graph = iswgraph(wc);
    bool lower = iswlower(wc);
    bool print = iswprint(wc);
    bool punct = iswpunct(wc);
    bool space = iswspace(wc);
    bool upper = iswupper(wc);
    bool xdigit = iswxdigit(wc);

    // Classes that are simply derived from others.
    ASSERT_EQ(alnum, alpha || digit);
    ASSERT_EQ(graph, alnum || punct);

    // Assertions derived from table in POSIX section 7.3.1 LC_CTYPE.
    // Automatically included.
    ASSERT_TRUE(!upper || alpha);
    ASSERT_TRUE(!upper || graph);
    ASSERT_TRUE(!upper || print);
    ASSERT_TRUE(!lower || alpha);
    ASSERT_TRUE(!lower || graph);
    ASSERT_TRUE(!lower || print);
    ASSERT_TRUE(!alpha || graph);
    ASSERT_TRUE(!alpha || print);
    ASSERT_TRUE(!digit || graph);
    ASSERT_TRUE(!digit || print);
    ASSERT_TRUE(!digit || xdigit);
    ASSERT_TRUE(!punct || graph);
    ASSERT_TRUE(!punct || print);
    ASSERT_TRUE(!graph || print);
    ASSERT_TRUE(!xdigit || graph);
    ASSERT_TRUE(!xdigit || print);
    ASSERT_TRUE(!blank || space);

    // Mutually exclusive:
    ASSERT_FALSE(upper && digit);
    ASSERT_FALSE(upper && space);
    ASSERT_FALSE(upper && cntrl);
    ASSERT_FALSE(upper && punct);
    ASSERT_FALSE(upper && blank);
    ASSERT_FALSE(lower && digit);
    ASSERT_FALSE(lower && space);
    ASSERT_FALSE(lower && cntrl);
    ASSERT_FALSE(lower && punct);
    ASSERT_FALSE(lower && blank);
    ASSERT_FALSE(alpha && digit);
    ASSERT_FALSE(alpha && space);
    ASSERT_FALSE(alpha && cntrl);
    ASSERT_FALSE(alpha && punct);
    ASSERT_FALSE(alpha && blank);
    ASSERT_FALSE(digit && space);
    ASSERT_FALSE(digit && cntrl);
    ASSERT_FALSE(digit && punct);
    ASSERT_FALSE(digit && blank);
    ASSERT_FALSE(space && xdigit);
    ASSERT_FALSE(cntrl && punct);
    ASSERT_FALSE(cntrl && graph);
    ASSERT_FALSE(cntrl && print);
    ASSERT_FALSE(cntrl && xdigit);
    ASSERT_FALSE(punct && xdigit);
    ASSERT_FALSE(xdigit && blank);
  }
}
