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

  // Boolean values. These should have a one-byte if true.
  TEST_YAML("\x02", "!!bool \"false\"");
  TEST_YAML("\x02Hello, world", "!!null \"null\"");
  TEST_YAML("\x02\x00", "!!null \"null\"");
  TEST_YAML("\x02\x01", "!!bool \"true\"");
  TEST_YAML("\x02\x02", "!!null \"null\"");

  // Integer values.
  TEST_YAML("\x05\xff\x7f\xff\xff\xff\xff\xff\xff\xff", "!!null \"null\"");
  TEST_YAML("\x05\x80\x00\x00\x00\x00\x00\x00\x00",
            "!!int \"-9223372036854775808\"");
  TEST_YAML("\x05\xfe\xff\xff\xff\xff\xff\xff\xff",
            "!!int \"-72057594037927937\"");
  TEST_YAML("\x05\xff\x00\x00\x00\x00\x00\x00\x00",
            "!!int \"-72057594037927936\"");
  TEST_YAML("\x05\xff\x7f\xff\xff\xff\xff\xff\xff",
            "!!int \"-36028797018963969\"");
  TEST_YAML("\x05\x80\x00\x00\x00\x00\x00\x00", "!!int \"-36028797018963968\"");
  TEST_YAML("\x05\xfe\xff\xff\xff\xff\xff\xff", "!!int \"-281474976710657\"");
  TEST_YAML("\x05\xff\x00\x00\x00\x00\x00\x00", "!!int \"-281474976710656\"");
  TEST_YAML("\x05\xff\x7f\xff\xff\xff\xff\xff", "!!int \"-140737488355329\"");
  TEST_YAML("\x05\x80\x00\x00\x00\x00\x00", "!!int \"-140737488355328\"");
  TEST_YAML("\x05\xfe\xff\xff\xff\xff\xff", "!!int \"-1099511627777\"");
  TEST_YAML("\x05\xff\x00\x00\x00\x00\x00", "!!int \"-1099511627776\"");
  TEST_YAML("\x05\xff\x7f\xff\xff\xff\xff", "!!int \"-549755813889\"");
  TEST_YAML("\x05\x80\x00\x00\x00\x00", "!!int \"-549755813888\"");
  TEST_YAML("\x05\xfe\xff\xff\xff\xff", "!!int \"-4294967297\"");
  TEST_YAML("\x05\xff\x00\x00\x00\x00", "!!int \"-4294967296\"");
  TEST_YAML("\x05\xff\x7f\xff\xff\xff", "!!int \"-2147483649\"");
  TEST_YAML("\x05\x80\x00\x00\x00", "!!int \"-2147483648\"");
  TEST_YAML("\x05\xfe\xff\xff\xff", "!!int \"-16777217\"");
  TEST_YAML("\x05\xff\x00\x00\x00", "!!int \"-16777216\"");
  TEST_YAML("\x05\xff\x7f\xff\xff", "!!int \"-8388609\"");
  TEST_YAML("\x05\x80\x00\x00", "!!int \"-8388608\"");
  TEST_YAML("\x05\xfe\xff\xff", "!!int \"-65537\"");
  TEST_YAML("\x05\xff\x00\x00", "!!int \"-65536\"");
  TEST_YAML("\x05\xff\x7f\xff", "!!int \"-32769\"");
  TEST_YAML("\x05\x80\x00", "!!int \"-32768\"");
  TEST_YAML("\x05\xfe\xff", "!!int \"-257\"");
  TEST_YAML("\x05\xff\x00", "!!int \"-256\"");
  TEST_YAML("\x05\xff\x7f", "!!int \"-129\"");
  TEST_YAML("\x05\x80", "!!int \"-128\"");
  TEST_YAML("\x05\xff", "!!int \"-1\"");
  TEST_YAML("\x05", "!!int \"0\"");
  TEST_YAML("\x05\x01", "!!int \"1\"");
  TEST_YAML("\x05\x7f", "!!int \"127\"");
  TEST_YAML("\x05\x00\x80", "!!int \"128\"");
  TEST_YAML("\x05\x00\xff", "!!int \"255\"");
  TEST_YAML("\x05\x01\x00", "!!int \"256\"");
  TEST_YAML("\x05\x7f\xff", "!!int \"32767\"");
  TEST_YAML("\x05\x00\x80\x00", "!!int \"32768\"");
  TEST_YAML("\x05\x00\xff\xff", "!!int \"65535\"");
  TEST_YAML("\x05\x01\x00\x00", "!!int \"65536\"");
  TEST_YAML("\x05\x7f\xff\xff", "!!int \"8388607\"");
  TEST_YAML("\x05\x00\x80\x00\x00", "!!int \"8388608\"");
  TEST_YAML("\x05\x00\xff\xff\xff", "!!int \"16777215\"");
  TEST_YAML("\x05\x01\x00\x00\x00", "!!int \"16777216\"");
  TEST_YAML("\x05\x7f\xff\xff\xff", "!!int \"2147483647\"");
  TEST_YAML("\x05\x00\x80\x00\x00\x00", "!!int \"2147483648\"");
  TEST_YAML("\x05\x00\xff\xff\xff\xff", "!!int \"4294967295\"");
  TEST_YAML("\x05\x01\x00\x00\x00\x00", "!!int \"4294967296\"");
  TEST_YAML("\x05\x7f\xff\xff\xff\xff", "!!int \"549755813887\"");
  TEST_YAML("\x05\x00\x80\x00\x00\x00\x00", "!!int \"549755813888\"");
  TEST_YAML("\x05\x00\xff\xff\xff\xff\xff", "!!int \"1099511627775\"");
  TEST_YAML("\x05\x01\x00\x00\x00\x00\x00", "!!int \"1099511627776\"");
  TEST_YAML("\x05\x7f\xff\xff\xff\xff\xff", "!!int \"140737488355327\"");
  TEST_YAML("\x05\x00\x80\x00\x00\x00\x00\x00", "!!int \"140737488355328\"");
  TEST_YAML("\x05\x00\xff\xff\xff\xff\xff\xff", "!!int \"281474976710655\"");
  TEST_YAML("\x05\x01\x00\x00\x00\x00\x00\x00", "!!int \"281474976710656\"");
  TEST_YAML("\x05\x7f\xff\xff\xff\xff\xff\xff", "!!int \"36028797018963967\"");
  TEST_YAML("\x05\x00\x80\x00\x00\x00\x00\x00\x00",
            "!!int \"36028797018963968\"");
  TEST_YAML("\x05\x00\xff\xff\xff\xff\xff\xff\xff",
            "!!int \"72057594037927935\"");
  TEST_YAML("\x05\x01\x00\x00\x00\x00\x00\x00\x00",
            "!!int \"72057594037927936\"");
  TEST_YAML("\x05\x7f\xff\xff\xff\xff\xff\xff\xff",
            "!!int \"9223372036854775807\"");
  TEST_YAML("\x05\x00\x80\x00\x00\x00\x00\x00\x00\x00",
            "!!int \"9223372036854775808\"");
  TEST_YAML("\x05\x00\xff\xff\xff\xff\xff\xff\xff\xff",
            "!!int \"18446744073709551615\"");
  TEST_YAML("\x05\x01\x00\x00\x00\x00\x00\x00\x00\x00", "!!null \"null\"");

  // Strings.
  // TODO(edje): Add tests for special characters and bad encoding.
  TEST_YAML("\x08", "!!null \"null\"");
  TEST_YAML("\x08Hello", "!!null \"null\"");
  TEST_YAML("\x08Hello\x00", "!!str \"Hello\"");

  // Maps.
  // TODO(ed): Add tests for multi-byte lengths.
  TEST_YAML("\x06", "!!map {}");
  TEST_YAML(
      "\x06"
      "\x87\x08Hello\x00\x87\x08World\x00"
      "\x81\x02\x82\x02\x01"
      "\x85\x05\x01\x80\x29\x15\x84\x05\x58\xe5\xd9"
      "\x80\x83\x06\x80\x80",
      "!!map {\n"
      "  ? !!str \"Hello\"\n"
      "  : !!str \"World\",\n"
      "  ? !!bool \"false\"\n"
      "  : !!bool \"true\",\n"
      "  ? !!int \"25176341\"\n"
      "  : !!int \"5826009\",\n"
      "  ? !!null \"null\"\n"
      "  : !!map {\n"
      "    ? !!null \"null\"\n"
      "    : !!null \"null\",\n"
      "  },\n"
      "}");

  // Sequences.
  TEST_YAML("\x07", "!!seq []");
  TEST_YAML(
      "\x07"
      "\x81\x02"
      "\x82\x02\x01"
      "\x80"
      "\x87\x08Hello\x00"
      "\x81\x06"
      "\x81\x07",
      "!!seq [\n"
      "  !!bool \"false\",\n"
      "  !!bool \"true\",\n"
      "  !!null \"null\",\n"
      "  !!str \"Hello\",\n"
      "  !!map {},\n"
      "  !!seq [],\n"
      "]");
#undef TEST_YAML
}
