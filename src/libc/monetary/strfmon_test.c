// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <monetary.h>
#include <testing.h>

TEST(strfmon, posix_examples) {
  // Examples from the description of strfmon() in POSIX.
  locale_t locale = newlocale(LC_MONETARY_MASK, "en_US", 0);
  ASSERT_NE(0, locale);

  char buf[15];
  ASSERT_EQ(7, strfmon_l(buf, sizeof(buf), locale, "%n", 123.45));
  ASSERT_STREQ("$123.45", buf);
  ASSERT_EQ(8, strfmon_l(buf, sizeof(buf), locale, "%n", -123.45));
  ASSERT_STREQ("-$123.45", buf);
  ASSERT_EQ(9, strfmon_l(buf, sizeof(buf), locale, "%n", 3456.781));
  ASSERT_STREQ("$3,456.78", buf);

  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%11n", 123.45));
  ASSERT_STREQ("    $123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%11n", -123.45));
  ASSERT_STREQ("   -$123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%11n", 3456.781));
  ASSERT_STREQ("  $3,456.78", buf);

  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%#5n", 123.45));
  ASSERT_STREQ(" $   123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%#5n", -123.45));
  ASSERT_STREQ("-$   123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%#5n", 3456.781));
  ASSERT_STREQ(" $ 3,456.78", buf);

  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%=*#5n", 123.45));
  ASSERT_STREQ(" $***123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%=*#5n", -123.45));
  ASSERT_STREQ("-$***123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%=*#5n", 3456.781));
  ASSERT_STREQ(" $*3,456.78", buf);

  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%=0#5n", 123.45));
  ASSERT_STREQ(" $000123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%=0#5n", -123.45));
  ASSERT_STREQ("-$000123.45", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%=0#5n", 3456.781));
  ASSERT_STREQ(" $03,456.78", buf);

  ASSERT_EQ(10, strfmon_l(buf, sizeof(buf), locale, "%^#5n", 123.45));
  ASSERT_STREQ(" $  123.45", buf);
  ASSERT_EQ(10, strfmon_l(buf, sizeof(buf), locale, "%^#5n", -123.45));
  ASSERT_STREQ("-$  123.45", buf);
  ASSERT_EQ(10, strfmon_l(buf, sizeof(buf), locale, "%^#5n", 3456.781));
  ASSERT_STREQ(" $ 3456.78", buf);

  ASSERT_EQ(7, strfmon_l(buf, sizeof(buf), locale, "%^#5.0n", 123.45));
  ASSERT_STREQ(" $  123", buf);
  ASSERT_EQ(7, strfmon_l(buf, sizeof(buf), locale, "%^#5.0n", -123.45));
  ASSERT_STREQ("-$  123", buf);
  ASSERT_EQ(7, strfmon_l(buf, sizeof(buf), locale, "%^#5.0n", 3456.781));
  ASSERT_STREQ(" $ 3457", buf);

  ASSERT_EQ(12, strfmon_l(buf, sizeof(buf), locale, "%^#5.4n", 123.45));
  ASSERT_STREQ(" $  123.4500", buf);
  ASSERT_EQ(12, strfmon_l(buf, sizeof(buf), locale, "%^#5.4n", -123.45));
  ASSERT_STREQ("-$  123.4500", buf);
  ASSERT_EQ(12, strfmon_l(buf, sizeof(buf), locale, "%^#5.4n", 3456.781));
  ASSERT_STREQ(" $ 3456.7810", buf);

  ASSERT_EQ(12, strfmon_l(buf, sizeof(buf), locale, "%(#5n", 123.45));
  ASSERT_STREQ(" $   123.45 ", buf);
  ASSERT_EQ(12, strfmon_l(buf, sizeof(buf), locale, "%(#5n", -123.45));
  ASSERT_STREQ("($   123.45)", buf);
  ASSERT_EQ(12, strfmon_l(buf, sizeof(buf), locale, "%(#5n", 3456.781));
  ASSERT_STREQ(" $ 3,456.78 ", buf);

  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%!(#5n", 123.45));
  ASSERT_STREQ("    123.45 ", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%!(#5n", -123.45));
  ASSERT_STREQ("(   123.45)", buf);
  ASSERT_EQ(11, strfmon_l(buf, sizeof(buf), locale, "%!(#5n", 3456.781));
  ASSERT_STREQ("  3,456.78 ", buf);

  ASSERT_EQ(14, strfmon_l(buf, sizeof(buf), locale, "%-14#5.4n", 123.45));
  ASSERT_STREQ(" $   123.4500 ", buf);
  ASSERT_EQ(14, strfmon_l(buf, sizeof(buf), locale, "%-14#5.4n", -123.45));
  ASSERT_STREQ("-$   123.4500 ", buf);
  ASSERT_EQ(14, strfmon_l(buf, sizeof(buf), locale, "%-14#5.4n", 3456.781));
  ASSERT_STREQ(" $ 3,456.7810 ", buf);

  ASSERT_EQ(14, strfmon_l(buf, sizeof(buf), locale, "%14#5.4n", 123.45));
  ASSERT_STREQ("  $   123.4500", buf);
  ASSERT_EQ(14, strfmon_l(buf, sizeof(buf), locale, "%14#5.4n", -123.45));
  ASSERT_STREQ(" -$   123.4500", buf);
  ASSERT_EQ(14, strfmon_l(buf, sizeof(buf), locale, "%14#5.4n", 3456.781));
  ASSERT_STREQ("  $ 3,456.7810", buf);

  freelocale(locale);
}
