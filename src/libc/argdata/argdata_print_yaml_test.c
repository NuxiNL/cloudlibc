// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <stdio.h>
#include <testing.h>

TEST(argdata_print_yaml, examples) {
#define TEST_YAML(in, out)                                      \
  do {                                                          \
    /* Prepare argument data input. */                          \
    argdata_t ad;                                               \
    argdata_init_binary(&ad, in, sizeof(in) - 1);               \
                                                                \
    /* Write YAML to output buffer. */                          \
    char outbuf[sizeof(out) + 5];                               \
    FILE *fp = fmemopen(outbuf, sizeof(outbuf), "w");           \
    ASSERT_NE(NULL, fp);                                        \
    ASSERT_EQ(0, argdata_print_yaml(&ad, fp));                  \
    ASSERT_EQ(sizeof(outbuf) - 1, ftello(fp));                  \
    ASSERT_EQ(0, fclose(fp));                                   \
                                                                \
    /* Compare against expected output. */                      \
    ASSERT_ARREQ("---\n" out "\n", outbuf, sizeof(outbuf) - 1); \
  } while (0)
  TEST_YAML("", "!!null \"null\"");

  // Binary data.
  TEST_YAML("\x01", "!!binary \"\"");
  TEST_YAML("\x01H", "!!binary \"SA==\"");
  TEST_YAML("\x01He", "!!binary \"SGU=\"");
  TEST_YAML("\x01Hel", "!!binary \"SGVs\"");
  TEST_YAML("\x01Hell", "!!binary \"SGVsbA==\"");
  TEST_YAML("\x01Hello", "!!binary \"SGVsbG8=\"");
  TEST_YAML("\x01Hello, world", "!!binary \"SGVsbG8sIHdvcmxk\"");

  // Boolean values. These should have a one-byte payload.
  TEST_YAML("\x02", "!!null \"null\"");
  TEST_YAML("\x02Hello, world", "!!null \"null\"");
  TEST_YAML("\x02\x00", "!!bool \"false\"");
  TEST_YAML("\x02\x01", "!!bool \"true\"");
  TEST_YAML("\x02\x02", "!!null \"null\"");
#undef TEST_YAML
}
