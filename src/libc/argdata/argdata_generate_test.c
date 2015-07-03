// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <stdint.h>
#include <testing.h>

#define TEST_OBJECT(obj, out)                                     \
  do {                                                            \
    ASSERT_EQ(sizeof(out) - 1, (obj)->length);                    \
    char outbuf[sizeof(out) - 1];                                 \
    int *fds;                                                     \
    size_t fdslen;                                                \
    ASSERT_EQ(0, __argdata_generate(obj, outbuf, &fds, &fdslen)); \
    ASSERT_ARREQ(out, outbuf, sizeof(outbuf));                    \
    ASSERT_EQ(NULL, fds);                                         \
    ASSERT_EQ(0, fdslen);                                         \
  } while (0)

TEST(argdata_generate, bool) {
  TEST_OBJECT(&argdata_false, "\x02");
  TEST_OBJECT(&argdata_true, "\x02\x01");
}

TEST(argdata_generate, int) {
#define TEST_INT(value, out)                   \
  do {                                         \
    argdata_t *ad = argdata_create_int(value); \
    TEST_OBJECT(ad, "\x05" out);               \
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

TEST(argdata_generate, null) {
  TEST_OBJECT(&argdata_null, "");
}
