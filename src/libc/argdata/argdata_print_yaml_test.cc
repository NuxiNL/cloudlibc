// Copyright (c) 2015-2016 Nuxi (https://nuxi.nl/) and contributors.
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <iterator>

#include "gtest/gtest.h"

#define TEST_OBJECT(obj, out)                         \
  do {                                                \
    /* Write YAML to output buffer. */                \
    char outbuf[sizeof(out) + 39];                    \
    FILE *fp = fmemopen(outbuf, sizeof(outbuf), "w"); \
    ASSERT_NE(NULL, fp);                              \
    argdata_print_yaml(obj, fp);                      \
    ASSERT_EQ(sizeof(outbuf) - 1, ftello(fp));        \
    ASSERT_EQ(0, fclose(fp));                         \
                                                      \
    /* Compare against expected output. */            \
    outbuf[sizeof(outbuf) - 1] = '\0';                \
    ASSERT_STREQ(                                     \
        "%TAG ! tag:nuxi.nl,2015:cloudabi/\n"         \
        "---\n" out "\n",                             \
        outbuf);                                      \
  } while (0)

static int fd_passthrough(void *arg, size_t fd) {
  return fd > INT_MAX ? -1 : fd;
}

TEST(argdata_print_yaml, buffer) {
#define TEST_BUFFER(in, out)                                           \
  do {                                                                 \
    /* Prepare argument data input. */                                 \
    argdata_t *ad =                                                    \
        argdata_from_buffer(in, sizeof(in) - 1, fd_passthrough, NULL); \
    TEST_OBJECT(ad, out);                                              \
    argdata_free(ad);                                                  \
  } while (0)
  TEST_BUFFER("", "!!null \"null\"");

  // Binary data.
  TEST_BUFFER("\x01", "!!binary \"\"");
  TEST_BUFFER("\x01H", "!!binary \"SA==\"");
  TEST_BUFFER("\x01He", "!!binary \"SGU=\"");
  TEST_BUFFER("\x01Hel", "!!binary \"SGVs\"");
  TEST_BUFFER("\x01Hell", "!!binary \"SGVsbA==\"");
  TEST_BUFFER("\x01Hello", "!!binary \"SGVsbG8=\"");
  TEST_BUFFER("\x01Hello, world", "!!binary \"SGVsbG8sIHdvcmxk\"");

  // Boolean values. These should have a one-byte if true.
  TEST_BUFFER("\x02", "!!bool \"false\"");
  TEST_BUFFER("\x02Hello, world", "!!null \"null\"");
  TEST_BUFFER("\x02\x00", "!!null \"null\"");
  TEST_BUFFER("\x02\x01", "!!bool \"true\"");
  TEST_BUFFER("\x02\x02", "!!null \"null\"");

  // File descriptors. These are fixed size. Invalid file descriptors
  // (in this case larger than INT_MAX) should be displayed as such.
  TEST_BUFFER("\x03", "!!null \"null\"");
  TEST_BUFFER("\x03\x00\x00\x00", "!!null \"null\"");
  TEST_BUFFER("\x03\x00\x00\x00\x00", "!fd \"0\"");
  TEST_BUFFER("\x03\x12\x34\x56\x78", "!fd \"305419896\"");
  TEST_BUFFER("\x03\xca\xfe\xba\xbe", "!fd \"invalid\"");
  TEST_BUFFER("\x03\x00\x00\x00\x00\x00", "!!null \"null\"");

  // Floating point values.
  TEST_BUFFER("\x04", "!!null \"null\"");
  TEST_BUFFER("\x04\x01\x02\x03\x04\x05\x06\x07", "!!null \"null\"");
  TEST_BUFFER("\x04\x01\x02\x03\x04\x05\x06\x07\x08\x09", "!!null \"null\"");
  TEST_BUFFER("\x04\xff\xf8\x00\x00\x00\x00\x00\x00", "!!float \".nan\"");
  TEST_BUFFER("\x04\xff\xf0\x00\x00\x00\x00\x00\x00", "!!float \"-.inf\"");
  TEST_BUFFER("\x04\xf4\xd6\x3b\xd6\xd8\x2c\x9f\xbc",
              "!!float \"-6.520321480035106e+254\"");
  TEST_BUFFER("\x04\xe7\xa7\x02\x9f\x9e\x47\x68\xc8",
              "!!float \"-2.05044707947561e+191\"");
  TEST_BUFFER("\x04\xc9\x58\x49\x5b\xc1\x67\x27\x02",
              "!!float \"-2.1664332176389768e+45\"");
  TEST_BUFFER("\x04\xb6\xf8\x3a\xfc\x88\x42\x44\x57",
              "!!float \"-6.790808900329073e-44\"");
  TEST_BUFFER("\x04\x8d\x7b\x0a\x8a\xba\x77\xbe\x3e",
              "!!float \"-9.900774043221159e-244\"");
  TEST_BUFFER("\x04\x80\x00\x00\x00\x00\x00\x00\x00", "!!float \"0\"");
  TEST_BUFFER("\x04\x00\x00\x00\x00\x00\x00\x00\x00", "!!float \"0\"");
  TEST_BUFFER("\x04\x11\xb6\x10\xcf\xc6\x59\x40\x9e",
              "!!float \"2.384516613265578e-223\"");
  TEST_BUFFER("\x04\x15\xd4\xa2\x9d\x7e\x09\x28\xd1",
              "!!float \"1.645401737310015e-203\"");
  TEST_BUFFER("\x04\x3f\xf0\x00\x00\x00\x00\x00\x00", "!!float \"1\"");
  TEST_BUFFER("\x04\x3f\xf8\x00\x00\x00\x00\x00\x00", "!!float \"1.5\"");
  TEST_BUFFER("\x04\x70\xcd\xe9\x27\xa4\x0f\x39\xf0",
              "!!float \"2.3775742871338425e+235\"");
  TEST_BUFFER("\x04\x74\x8e\x1a\xe1\xa2\x3f\xf6\xfc",
              "!!float \"2.75895794262241e+253\"");
  TEST_BUFFER("\x04\x75\xaa\xe8\x5c\x1c\x37\x89\x00",
              "!!float \"6.464309645384425e+258\"");
  TEST_BUFFER("\x04\x7f\xf0\x00\x00\x00\x00\x00\x00", "!!float \".inf\"");
  TEST_BUFFER("\x04\x7f\xf8\x00\x00\x00\x00\x00\x00", "!!float \".nan\"");

  // Integer values.
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff\xff\xff\xff", "!!null \"null\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"-9223372036854775808\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"-72057594037927937\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"-72057594037927936\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff\xff\xff",
              "!!int \"-36028797018963969\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00\x00\x00",
              "!!int \"-36028797018963968\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff\xff\xff", "!!int \"-281474976710657\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00\x00\x00", "!!int \"-281474976710656\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff\xff", "!!int \"-140737488355329\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00\x00", "!!int \"-140737488355328\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff\xff", "!!int \"-1099511627777\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00\x00", "!!int \"-1099511627776\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff", "!!int \"-549755813889\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00", "!!int \"-549755813888\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff", "!!int \"-4294967297\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00", "!!int \"-4294967296\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff", "!!int \"-2147483649\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00", "!!int \"-2147483648\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff", "!!int \"-16777217\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00", "!!int \"-16777216\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff", "!!int \"-8388609\"");
  TEST_BUFFER("\x05\x80\x00\x00", "!!int \"-8388608\"");
  TEST_BUFFER("\x05\xfe\xff\xff", "!!int \"-65537\"");
  TEST_BUFFER("\x05\xff\x00\x00", "!!int \"-65536\"");
  TEST_BUFFER("\x05\xff\x7f\xff", "!!int \"-32769\"");
  TEST_BUFFER("\x05\x80\x00", "!!int \"-32768\"");
  TEST_BUFFER("\x05\xfe\xff", "!!int \"-257\"");
  TEST_BUFFER("\x05\xff\x00", "!!int \"-256\"");
  TEST_BUFFER("\x05\xff\x7f", "!!int \"-129\"");
  TEST_BUFFER("\x05\x80", "!!int \"-128\"");
  TEST_BUFFER("\x05\xff", "!!int \"-1\"");
  TEST_BUFFER("\x05", "!!int \"0\"");
  TEST_BUFFER("\x05\x01", "!!int \"1\"");
  TEST_BUFFER("\x05\x7f", "!!int \"127\"");
  TEST_BUFFER("\x05\x00\x80", "!!int \"128\"");
  TEST_BUFFER("\x05\x00\xff", "!!int \"255\"");
  TEST_BUFFER("\x05\x01\x00", "!!int \"256\"");
  TEST_BUFFER("\x05\x7f\xff", "!!int \"32767\"");
  TEST_BUFFER("\x05\x00\x80\x00", "!!int \"32768\"");
  TEST_BUFFER("\x05\x00\xff\xff", "!!int \"65535\"");
  TEST_BUFFER("\x05\x01\x00\x00", "!!int \"65536\"");
  TEST_BUFFER("\x05\x7f\xff\xff", "!!int \"8388607\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00", "!!int \"8388608\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff", "!!int \"16777215\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00", "!!int \"16777216\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff", "!!int \"2147483647\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00", "!!int \"2147483648\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff", "!!int \"4294967295\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00", "!!int \"4294967296\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff", "!!int \"549755813887\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00", "!!int \"549755813888\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff", "!!int \"1099511627775\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00", "!!int \"1099511627776\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff\xff", "!!int \"140737488355327\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00\x00", "!!int \"140737488355328\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff\xff", "!!int \"281474976710655\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00\x00", "!!int \"281474976710656\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff\xff\xff",
              "!!int \"36028797018963967\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00\x00\x00",
              "!!int \"36028797018963968\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"72057594037927935\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"72057594037927936\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"9223372036854775807\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"9223372036854775808\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"18446744073709551615\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00\x00\x00\x00", "!!null \"null\"");

  // Strings.
  TEST_BUFFER("\x08", "!!null \"null\"");
  TEST_BUFFER("\x08Hello", "!!null \"null\"");
  TEST_BUFFER("\x08Hello\x00", "!!str \"Hello\"");
  TEST_BUFFER("\x08Invalid\xffUnicode\x00", "!!null \"null\"");
  TEST_BUFFER("\x08\a\b\n\r\x1b\0\x03☃\x00",
              "!!str \"\\a\\b\\n\\r\\e\\0\\x03☃\"");

  // Maps.
  // TODO(ed): Add tests for multi-byte lengths.
  TEST_BUFFER("\x06", "!!map {}");
  TEST_BUFFER(
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
  TEST_BUFFER("\x07", "!!seq []");
  TEST_BUFFER(
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

  // Timestamps.
  TEST_BUFFER("\x09\x13\xf0\xbc\xeb\x1a\x26\x3c\x69",
              "!!timestamp \"2015-07-14T06:39:59.073238121Z\"");
#undef TEST_BUFFER
}

TEST(argdata_print_yaml, binary) {
  argdata_t *ad = argdata_create_binary(NULL, 0);
  TEST_OBJECT(ad, "!!binary \"\"");
  argdata_free(ad);

  ad = argdata_create_binary("This is an example string", 25);
  TEST_OBJECT(ad, "!!binary \"VGhpcyBpcyBhbiBleGFtcGxlIHN0cmluZw==\"");
  argdata_free(ad);
}

TEST(argdata_print_yaml, bool) {
  TEST_OBJECT(&argdata_false, "!!bool \"false\"");
  TEST_OBJECT(&argdata_true, "!!bool \"true\"");
}

TEST(argdata_print_yaml, fd) {
  ASSERT_EQ(NULL, argdata_create_fd(-1));
  ASSERT_EQ(EBADF, errno);

  argdata_t *ad = argdata_create_fd(12345678);
  TEST_OBJECT(ad, "!fd \"12345678\"");
  argdata_free(ad);
}

TEST(argdata_print_yaml, int) {
#define TEST_INT(value, out)                   \
  do {                                         \
    argdata_t *ad = argdata_create_int(value); \
    TEST_OBJECT(ad, "!!int \"" out "\"");      \
    argdata_free(ad);                          \
  } while (0)
  TEST_INT(0, "0");
  TEST_INT(0xdeadc0de, "3735929054");
  TEST_INT(INT64_MIN, "-9223372036854775808");
  TEST_INT(INT64_MAX, "9223372036854775807");
  TEST_INT(UINT64_MAX, "18446744073709551615");
#undef TEST_INT
}

TEST(argdata_print_yaml, map) {
  {
    argdata_t *ad = argdata_create_map(NULL, NULL, 0);
    TEST_OBJECT(ad, "!!map {}");
    argdata_free(ad);
  }

  {
    const argdata_t *keys[] = {&argdata_true, &argdata_false, &argdata_null};
    const argdata_t *values[] = {&argdata_null, &argdata_true, &argdata_false};
    argdata_t *ad = argdata_create_map(keys, values, std::size(keys));
    TEST_OBJECT(ad,
                "!!map {\n"
                "  ? !!bool \"true\"\n"
                "  : !!null \"null\",\n"
                "  ? !!bool \"false\"\n"
                "  : !!bool \"true\",\n"
                "  ? !!null \"null\"\n"
                "  : !!bool \"false\",\n"
                "}");
    argdata_free(ad);
  }
}

TEST(argdata_print_yaml, null) {
  TEST_OBJECT(&argdata_null, "!!null \"null\"");
}

TEST(argdata_print_yaml, seq) {
  {
    argdata_t *ad = argdata_create_seq(NULL, 0);
    TEST_OBJECT(ad, "!!seq []");
    argdata_free(ad);
  }

  {
    const argdata_t *entries[] = {&argdata_false, &argdata_null, &argdata_true};
    argdata_t *ad = argdata_create_seq(entries, std::size(entries));
    TEST_OBJECT(ad,
                "!!seq [\n"
                "  !!bool \"false\",\n"
                "  !!null \"null\",\n"
                "  !!bool \"true\",\n"
                "]");
    argdata_free(ad);
  }
}

TEST(argdata_print_yaml, str) {
  argdata_t *ad = argdata_create_str_c("Hello, world");
  TEST_OBJECT(ad, "!!str \"Hello, world\"");
  argdata_free(ad);
}
