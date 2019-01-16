// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <iterator>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

static int fd_passthrough(void *arg, size_t fd) {
  return fd > INT_MAX ? -1 : fd;
}

#define TEST_OBJECT(obj, out, nfds, ...)                                       \
  do {                                                                         \
    std::string edata(out, sizeof(out) - 1);                                   \
    std::vector<int> efds({__VA_ARGS__});                                      \
                                                                               \
    /* Compute size of resulting code. */                                      \
    size_t datalen;                                                            \
    size_t fdslen;                                                             \
    argdata_serialized_length(obj, &datalen, &fdslen);                         \
    ASSERT_EQ(edata.size(), datalen);                                          \
    ASSERT_EQ(nfds, fdslen);                                                   \
                                                                               \
    /* Generate code and compare. */                                           \
    std::string data(edata.size(), '\0');                                      \
    std::vector<int> fds(efds.size());                                         \
    ASSERT_EQ(efds.size(), argdata_serialize(obj, data.data(), fds.data()));   \
    ASSERT_EQ(edata, data);                                                    \
    ASSERT_EQ(efds, fds);                                                      \
                                                                               \
    /* Generating twice should not output different code. */                   \
    argdata_t *ad2 =                                                           \
        argdata_from_buffer(edata.data(), edata.size(), fd_passthrough, NULL); \
    argdata_serialized_length(ad2, &datalen, &fdslen);                         \
    ASSERT_EQ(edata.size(), datalen);                                          \
    ASSERT_EQ(nfds, fdslen);                                                   \
    ASSERT_EQ(efds.size(), argdata_serialize(ad2, data.data(), fds.data()));   \
    argdata_free(ad2);                                                         \
    ASSERT_THAT(edata, data);                                                  \
    for (size_t i = 0; i < efds.size(); ++i)                                   \
      ASSERT_EQ(i, fds[i]);                                                    \
  } while (0)

TEST(argdata_serialize, buffer) {
#define TEST_BUFFER(in, out, nfds, ...)                                \
  do {                                                                 \
    argdata_t *ad =                                                    \
        argdata_from_buffer(in, sizeof(in) - 1, fd_passthrough, NULL); \
    TEST_OBJECT(ad, out, nfds, ##__VA_ARGS__);                         \
    argdata_free(ad);                                                  \
  } while (0)
  // Null.
  TEST_BUFFER("", "", 0);

  // Binary data.
  TEST_BUFFER("\x01", "\x01", 0);
  TEST_BUFFER("\x01Hello", "\x01Hello", 0);

  // Boolean values.
  TEST_BUFFER("\x02", "\x02", 0);
  TEST_BUFFER("\x02Hello, world", "\x02Hello, world", 0);
  TEST_BUFFER("\x02\x00", "\x02\x00", 0);
  TEST_BUFFER("\x02\x01", "\x02\x01", 0);
  TEST_BUFFER("\x02\x02", "\x02\x02", 0);

  // File descriptors. With the fd_passthrough function provided, any
  // invalid file descriptor should be remapped to UINT32_MAX, so that
  // it remains invalid when deserialized.
  TEST_BUFFER("\x03", "\x03", 0);
  TEST_BUFFER("\x03\x00\x00\x00", "\x03\x00\x00\x00", 0);
  TEST_BUFFER("\x03\x00\x00\x00\x00", "\x03\x00\x00\x00\x00", 1, 0);
  TEST_BUFFER("\x03\x00\x00\x00\x13", "\x03\x00\x00\x00\x00", 1, 19);
  TEST_BUFFER("\x03\xca\xfe\xba\xbe", "\x03\xff\xff\xff\xff", 1);

  // Integer values.
  TEST_BUFFER("\x05Integers should not get modified",
              "\x05Integers should not get modified", 0);

  // Strings.
  TEST_BUFFER("\x08Invalid\x80Unicode? Not a problem!\x00",
              "\x08Invalid\x80Unicode? Not a problem!\x00", 0);
  TEST_BUFFER("\x08Improperly terminated string",
              "\x08Improperly terminated string", 0);

  // Maps. File descriptors should get remapped in sorted order.
  TEST_BUFFER(
      "\x06"
      "\x85\x03\x12\x34\x56\x78\x85\x03\x01\x23\x45\x67"
      "\x85\x03\x01\x23\x45\x67\x85\x03\x00\x00\x00\x12"
      "Trailing garbage that is not processed\x85\x03\x00\x00\x00\x42",
      "\x06"
      "\x85\x03\x00\x00\x00\x00\x85\x03\x00\x00\x00\x01"
      "\x85\x03\x00\x00\x00\x01\x85\x03\x00\x00\x00\x02"
      "Trailing garbage that is not processed\x85\x03\x00\x00\x00\x42",
      4, 305419896, 19088743, 18);

  // Random garbage shouldn't get modified.
  TEST_BUFFER("Some random data", "Some random data", 0);
#undef TEST_BUFFER
}

TEST(argdata_serialize, binary) {
  argdata_t *ad = argdata_create_binary("Hello", 5);
  TEST_OBJECT(ad, "\x01Hello", 0);
  argdata_free(ad);
}

TEST(argdata_serialize, bool) {
  TEST_OBJECT(&argdata_false, "\x02", 0);
  TEST_OBJECT(&argdata_true, "\x02\x01", 0);
}

TEST(argdata_serialize, fd) {
  argdata_t *ad = argdata_create_fd(12);
  TEST_OBJECT(ad, "\x03\x00\x00\x00\x00", 1, 12);
  argdata_free(ad);
}

TEST(argdata_serialize, float) {
#define TEST_FLOAT(value, out)                   \
  do {                                           \
    argdata_t *ad = argdata_create_float(value); \
    TEST_OBJECT(ad, "\x04" out, 0);              \
    argdata_free(ad);                            \
  } while (0)
  TEST_FLOAT(-INFINITY, "\xff\xf0\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(-0x1.e56b76eacd009p+54, "\xc3\x5e\x56\xb7\x6e\xac\xd0\x09");
  TEST_FLOAT(-2.0, "\xc0\x00\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(-1.0, "\xbf\xf0\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(-0.0, "\x80\x00\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(0.0, "\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(0x1.6948c90b03bdcp-191, "\x34\x06\x94\x8c\x90\xb0\x3b\xdc");
  TEST_FLOAT(1.0, "\x3f\xf0\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(0x1.0000000000001p+0, "\x3f\xf0\x00\x00\x00\x00\x00\x01");
  TEST_FLOAT(0x1.0000000000002p+0, "\x3f\xf0\x00\x00\x00\x00\x00\x02");
  TEST_FLOAT(2.0, "\x40\x00\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(INFINITY, "\x7f\xf0\x00\x00\x00\x00\x00\x00");
  TEST_FLOAT(NAN, "\x7f\xf8\x00\x00\x00\x00\x00\x00");
#undef TEST_FLOAT
}

TEST(argdata_serialize, int) {
#define TEST_INT(value, out)                   \
  do {                                         \
    argdata_t *ad = argdata_create_int(value); \
    TEST_OBJECT(ad, "\x05" out, 0);            \
    argdata_free(ad);                          \
  } while (0)
  TEST_INT(INT64_MIN, "\x80\x00\x00\x00\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-72057594037927937), "\xfe\xff\xff\xff\xff\xff\xff\xff");
  TEST_INT(INTMAX_C(-72057594037927936), "\xff\x00\x00\x00\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-36028797018963969), "\xff\x7f\xff\xff\xff\xff\xff\xff");
  TEST_INT(INTMAX_C(-36028797018963968), "\x80\x00\x00\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-281474976710657), "\xfe\xff\xff\xff\xff\xff\xff");
  TEST_INT(INTMAX_C(-281474976710656), "\xff\x00\x00\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-140737488355329), "\xff\x7f\xff\xff\xff\xff\xff");
  TEST_INT(INTMAX_C(-140737488355328), "\x80\x00\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-1099511627777), "\xfe\xff\xff\xff\xff\xff");
  TEST_INT(INTMAX_C(-1099511627776), "\xff\x00\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-549755813889), "\xff\x7f\xff\xff\xff\xff");
  TEST_INT(INTMAX_C(-549755813888), "\x80\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-4294967297), "\xfe\xff\xff\xff\xff");
  TEST_INT(INTMAX_C(-4294967296), "\xff\x00\x00\x00\x00");
  TEST_INT(INTMAX_C(-2147483649), "\xff\x7f\xff\xff\xff");
  TEST_INT(INT32_MIN, "\x80\x00\x00\x00");
  TEST_INT(INTMAX_C(-16777217), "\xfe\xff\xff\xff");
  TEST_INT(INTMAX_C(-16777216), "\xff\x00\x00\x00");
  TEST_INT(INTMAX_C(-8388609), "\xff\x7f\xff\xff");
  TEST_INT(INTMAX_C(-8388608), "\x80\x00\x00");
  TEST_INT(INTMAX_C(-65537), "\xfe\xff\xff");
  TEST_INT(INTMAX_C(-65536), "\xff\x00\x00");
  TEST_INT(INTMAX_C(-32769), "\xff\x7f\xff");
  TEST_INT(INT16_MIN, "\x80\x00");
  TEST_INT(INTMAX_C(-257), "\xfe\xff");
  TEST_INT(INTMAX_C(-256), "\xff\x00");
  TEST_INT(INTMAX_C(-129), "\xff\x7f");
  TEST_INT(INT8_MIN, "\x80");
  TEST_INT(INTMAX_C(-1), "\xff");
  TEST_INT(UINTMAX_C(0), "");
  TEST_INT(UINTMAX_C(1), "\x01");
  TEST_INT(INT8_MAX, "\x7f");
  TEST_INT(UINTMAX_C(128), "\x00\x80");
  TEST_INT(UINT8_MAX, "\x00\xff");
  TEST_INT(UINTMAX_C(256), "\x01\x00");
  TEST_INT(INT16_MAX, "\x7f\xff");
  TEST_INT(UINTMAX_C(32768), "\x00\x80\x00");
  TEST_INT(UINT16_MAX, "\x00\xff\xff");
  TEST_INT(UINTMAX_C(65536), "\x01\x00\x00");
  TEST_INT(UINTMAX_C(8388607), "\x7f\xff\xff");
  TEST_INT(UINTMAX_C(8388608), "\x00\x80\x00\x00");
  TEST_INT(UINTMAX_C(16777215), "\x00\xff\xff\xff");
  TEST_INT(UINTMAX_C(16777216), "\x01\x00\x00\x00");
  TEST_INT(INT32_MAX, "\x7f\xff\xff\xff");
  TEST_INT(UINTMAX_C(2147483648), "\x00\x80\x00\x00\x00");
  TEST_INT(UINT32_MAX, "\x00\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(4294967296), "\x01\x00\x00\x00\x00");
  TEST_INT(UINTMAX_C(549755813887), "\x7f\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(549755813888), "\x00\x80\x00\x00\x00\x00");
  TEST_INT(UINTMAX_C(1099511627775), "\x00\xff\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(1099511627776), "\x01\x00\x00\x00\x00\x00");
  TEST_INT(UINTMAX_C(140737488355327), "\x7f\xff\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(140737488355328), "\x00\x80\x00\x00\x00\x00\x00");
  TEST_INT(UINTMAX_C(281474976710655), "\x00\xff\xff\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(281474976710656), "\x01\x00\x00\x00\x00\x00\x00");
  TEST_INT(UINTMAX_C(36028797018963967), "\x7f\xff\xff\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(36028797018963968), "\x00\x80\x00\x00\x00\x00\x00\x00");
  TEST_INT(UINTMAX_C(72057594037927935), "\x00\xff\xff\xff\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(72057594037927936), "\x01\x00\x00\x00\x00\x00\x00\x00");
  TEST_INT(INT64_MAX, "\x7f\xff\xff\xff\xff\xff\xff\xff");
  TEST_INT(UINTMAX_C(9223372036854775808),
           "\x00\x80\x00\x00\x00\x00\x00\x00\x00");
  TEST_INT(UINT64_MAX, "\x00\xff\xff\xff\xff\xff\xff\xff\xff");
#undef TEST_INT
}

TEST(argdata_serialize, map) {
  {
    argdata_t *ad = argdata_create_map(NULL, NULL, 0);
    TEST_OBJECT(ad, "\x06", 0);
    argdata_free(ad);
  }

  {
    const argdata_t *keys[] = {&argdata_true, &argdata_false, &argdata_null};
    const argdata_t *values[] = {&argdata_null, &argdata_true, &argdata_false};
    argdata_t *ad = argdata_create_map(keys, values, std::size(keys));
    TEST_OBJECT(ad, "\x06\x82\x02\x01\x80\x81\x02\x82\x02\x01\x80\x81\x02", 0);
    argdata_free(ad);
  }
}

TEST(argdata_serialize, null) {
  TEST_OBJECT(&argdata_null, "", 0);
}

TEST(argdata_serialize, seq) {
  {
    argdata_t *ad = argdata_create_seq(NULL, 0);
    TEST_OBJECT(ad, "\x07", 0);
    argdata_free(ad);
  }

  {
    const argdata_t *entries[] = {&argdata_false, &argdata_null, &argdata_true};
    argdata_t *ad = argdata_create_seq(entries, std::size(entries));
    TEST_OBJECT(ad, "\x07\x81\x02\x80\x82\x02\x01", 0);
    argdata_free(ad);
  }

  {
    argdata_t *str = argdata_create_str_c(
        "This is a quite long string whose length is exactly 125 bytes long. "
        "This way its length can be encoded in seven bits of data.");
    const argdata_t *entries[] = {&argdata_null, str, &argdata_null};
    argdata_t *ad = argdata_create_seq(entries, std::size(entries));
    TEST_OBJECT(ad,
                "\x07\x80\xff\x08This is a quite long string whose length is "
                "exactly 125 bytes long. This way its length can be encoded in "
                "seven bits of data.\x00\x80",
                0);
    argdata_free(str);
    argdata_free(ad);
  }

  {
    argdata_t *str = argdata_create_str_c(
        "This is a very long string whose length is exactly 126 bytes long. "
        "This way its length can't be encoded in seven bits of data.");
    const argdata_t *entries[] = {&argdata_null, str, &argdata_null};
    argdata_t *ad = argdata_create_seq(entries, std::size(entries));
    TEST_OBJECT(
        ad,
        "\x07\x80\x01\x80\x08This is a very long string whose length is "
        "exactly 126 bytes long. This way its length can't be encoded in "
        "seven bits of data.\x00\x80",
        0);
    argdata_free(str);
    argdata_free(ad);
  }
}

TEST(argdata_serialize, str) {
  argdata_t *ad = argdata_create_str("Hello\x00world", 11);
  TEST_OBJECT(ad, "\x08Hello\x00world\x00", 0);
  argdata_free(ad);
}

TEST(argdata_serialize, timestamp) {
#define TEST_TIMESTAMP(s, ns, out)                          \
  do {                                                      \
    struct timespec ts1 = {.tv_sec = (s), .tv_nsec = (ns)}; \
    argdata_t *ad = argdata_create_timestamp(&ts1);         \
    TEST_OBJECT(ad, "\x09" out, 0);                         \
                                                            \
    struct timespec ts2;                                    \
    ASSERT_EQ(0, argdata_get_timestamp(ad, &ts2));          \
    ASSERT_EQ(ts1.tv_sec, ts2.tv_sec);                      \
    ASSERT_EQ(ts1.tv_nsec, ts2.tv_nsec);                    \
    argdata_free(ad);                                       \
  } while (0)
  TEST_TIMESTAMP(INT64_MIN, 0,
                 "\xe2\x32\x9b\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-309485009821345069), 275218943,
                 "\xfe\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-309485009821345069), 275218944,
                 "\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-154742504910672535), 637609471,
                 "\xff\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-154742504910672535), 637609472,
                 "\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-1208925819614630), 825293823,
                 "\xfe\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-1208925819614630), 825293824,
                 "\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-604462909807315), 412646911,
                 "\xff\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-604462909807315), 412646912,
                 "\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-4722366482870), 354786303,
                 "\xfe\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-4722366482870), 354786304,
                 "\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-2361183241435), 177393151,
                 "\xff\x7f\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-2361183241435), 177393152,
                 "\x80\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-18446744074), 290448383,
                 "\xfe\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-18446744074), 290448384,
                 "\xff\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-9223372037), 145224191,
                 "\xff\x7f\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-9223372037), 145224192,
                 "\x80\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-72057595), 962072063,
                 "\xfe\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-72057595), 962072064,
                 "\xff\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-36028798), 981036031,
                 "\xff\x7f\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-36028798), 981036032, "\x80\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-281475), 23289343, "\xfe\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-281475), 23289344, "\xff\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-140738), 511644671, "\xff\x7f\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-140738), 511644672, "\x80\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-1100), 488372223, "\xfe\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-1100), 488372224, "\xff\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-550), 244186111, "\xff\x7f\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-550), 244186112, "\x80\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-5), 705032703, "\xfe\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-5), 705032704, "\xff\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-3), 852516351, "\xff\x7f\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-3), 852516352, "\x80\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-1), 983222783, "\xfe\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(-1), 983222784, "\xff\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(-1), 991611391, "\xff\x7f\xff\xff");
  TEST_TIMESTAMP(INT64_C(-1), 991611392, "\x80\x00\x00");
  TEST_TIMESTAMP(INT64_C(-1), 999934463, "\xfe\xff\xff");
  TEST_TIMESTAMP(INT64_C(-1), 999934464, "\xff\x00\x00");
  TEST_TIMESTAMP(INT64_C(-1), 999967231, "\xff\x7f\xff");
  TEST_TIMESTAMP(INT64_C(-1), 999967232, "\x80\x00");
  TEST_TIMESTAMP(INT64_C(-1), 999999743, "\xfe\xff");
  TEST_TIMESTAMP(INT64_C(-1), 999999744, "\xff\x00");
  TEST_TIMESTAMP(INT64_C(-1), 999999871, "\xff\x7f");
  TEST_TIMESTAMP(INT64_C(-1), 999999872, "\x80");
  TEST_TIMESTAMP(INT64_C(-1), 999999999, "\xff");
  TEST_TIMESTAMP(INT64_C(0), 0, "");
  TEST_TIMESTAMP(INT64_C(0), 1, "\x01");
  TEST_TIMESTAMP(INT64_C(0), 127, "\x7f");
  TEST_TIMESTAMP(INT64_C(0), 128, "\x00\x80");
  TEST_TIMESTAMP(INT64_C(0), 255, "\x00\xff");
  TEST_TIMESTAMP(INT64_C(0), 256, "\x01\x00");
  TEST_TIMESTAMP(INT64_C(0), 32767, "\x7f\xff");
  TEST_TIMESTAMP(INT64_C(0), 32768, "\x00\x80\x00");
  TEST_TIMESTAMP(INT64_C(0), 65535, "\x00\xff\xff");
  TEST_TIMESTAMP(INT64_C(0), 65536, "\x01\x00\x00");
  TEST_TIMESTAMP(INT64_C(0), 8388607, "\x7f\xff\xff");
  TEST_TIMESTAMP(INT64_C(0), 8388608, "\x00\x80\x00\x00");
  TEST_TIMESTAMP(INT64_C(0), 16777215, "\x00\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(0), 16777216, "\x01\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(2), 147483647, "\x7f\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(2), 147483648, "\x00\x80\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(4), 294967295, "\x00\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(4), 294967296, "\x01\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(549), 755813887, "\x7f\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(549), 755813888, "\x00\x80\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(1099), 511627775, "\x00\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(1099), 511627776, "\x01\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(140737), 488355327, "\x7f\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(140737), 488355328, "\x00\x80\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(281474), 976710655, "\x00\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(281474), 976710656, "\x01\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(36028797), 18963967, "\x7f\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(36028797), 18963968,
                 "\x00\x80\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(72057594), 37927935,
                 "\x00\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(72057594), 37927936,
                 "\x01\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(9223372036), 854775807,
                 "\x7f\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(9223372036), 854775808,
                 "\x00\x80\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(18446744073), 709551615,
                 "\x00\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(18446744073), 709551616,
                 "\x01\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(2361183241434), 822606847,
                 "\x7f\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(2361183241434), 822606848,
                 "\x00\x80\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(4722366482869), 645213695,
                 "\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(4722366482869), 645213696,
                 "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(604462909807314), 587353087,
                 "\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(604462909807314), 587353088,
                 "\x00\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(1208925819614629), 174706175,
                 "\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(1208925819614629), 174706176,
                 "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(154742504910672534), 362390527,
                 "\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(154742504910672534), 362390528,
                 "\x00\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_C(309485009821345068), 724781055,
                 "\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff");
  TEST_TIMESTAMP(INT64_C(309485009821345068), 724781056,
                 "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_TIMESTAMP(INT64_MAX, 999999999,
                 "\x1d\xcd\x64\xff\xff\xff\xff\xff\xff\xff\xff\xff");
#undef TEST_TIMESTAMP
}
