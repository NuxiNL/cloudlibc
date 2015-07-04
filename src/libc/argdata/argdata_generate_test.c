// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <stdint.h>
#include <testing.h>

#define TEST_OBJECT(obj, out, nfds, ...)                                \
  do {                                                                  \
    /* Compute size of resulting code. */                               \
    size_t datalen;                                                     \
    size_t fdslen;                                                      \
    __argdata_getspace(obj, &datalen, &fdslen);                         \
    ASSERT_EQ(sizeof(out) - 1, datalen);                                \
    ASSERT_EQ(nfds, fdslen);                                            \
                                                                        \
    /* Generate code and compare. */                                    \
    char data[sizeof(out) - 1];                                         \
    int fds[nfds];                                                      \
    int efds[] = {__VA_ARGS__};                                         \
    ASSERT_EQ(__arraycount(efds), __argdata_generate(obj, data, fds));  \
    ASSERT_ARREQ(out, data, sizeof(data));                              \
    ASSERT_ARREQ(efds, fds, __arraycount(efds));                        \
                                                                        \
    /* Generating twice should not output different code. */            \
    argdata_t ad2;                                                      \
    argdata_init_binary(&ad2, out, sizeof(out) - 1);                    \
    __argdata_getspace(&ad2, &datalen, &fdslen);                        \
    ASSERT_EQ(sizeof(out) - 1, datalen);                                \
    ASSERT_EQ(nfds, fdslen);                                            \
    ASSERT_EQ(__arraycount(efds), __argdata_generate(&ad2, data, fds)); \
    ASSERT_ARREQ(out, data, sizeof(data));                              \
    for (size_t i = 0; i < (nfds); ++i)                                 \
      ASSERT_EQ(i, fds[i]);                                             \
  } while (0)

TEST(argdata_generate, binary) {
  argdata_t *ad = argdata_create_binary("Hello", 5);
  TEST_OBJECT(ad, "\x01Hello", 0);
  argdata_free(ad);
}

TEST(argdata_generate, bool) {
  TEST_OBJECT(&argdata_false, "\x02", 0);
  TEST_OBJECT(&argdata_true, "\x02\x01", 0);
}

TEST(argdata_generate, fd) {
  argdata_t *ad = argdata_create_fd(12);
  TEST_OBJECT(ad, "\x03\x00\x00\x00\x00", 1, 12);
  argdata_free(ad);
}

TEST(argdata_generate, int) {
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

TEST(argdata_print_yaml, map) {
  {
    argdata_t *ad = argdata_create_map(NULL, NULL, 0);
    TEST_OBJECT(ad, "\x06", 0);
    argdata_free(ad);
  }

  {
    const argdata_t *keys[] = {&argdata_true, &argdata_false, &argdata_null};
    const argdata_t *values[] = {&argdata_null, &argdata_true, &argdata_false};
    argdata_t *ad = argdata_create_map(keys, values, __arraycount(keys));
    TEST_OBJECT(ad, "\x06\x82\x02\x01\x80\x81\x02\x82\x02\x01\x80\x81\x02", 0);
    argdata_free(ad);
  }
}

TEST(argdata_generate, null) {
  TEST_OBJECT(&argdata_null, "", 0);
}

TEST(argdata_generate, seq) {
  {
    argdata_t *ad = argdata_create_seq(NULL, 0);
    TEST_OBJECT(ad, "\x07", 0);
    argdata_free(ad);
  }

  {
    const argdata_t *entries[] = {&argdata_false, &argdata_null, &argdata_true};
    argdata_t *ad = argdata_create_seq(entries, __arraycount(entries));
    TEST_OBJECT(ad, "\x07\x81\x02\x80\x82\x02\x01", 0);
    argdata_free(ad);
  }

  {
    argdata_t *str = argdata_create_str_c(
        "This is a quite long string whose length is exactly 125 bytes long. "
        "This way its length can be encoded in seven bits of data.");
    const argdata_t *entries[] = {&argdata_null, str, &argdata_null};
    argdata_t *ad = argdata_create_seq(entries, __arraycount(entries));
    TEST_OBJECT(ad,
                "\x07\x80\xff\x08This is a quite long string whose length is "
                "exactly 125 bytes long. This way its length can be encoded in "
                "seven bits of data.\x00\x80",
                0);
    argdata_free(ad);
  }

  {
    argdata_t *str = argdata_create_str_c(
        "This is a very long string whose length is exactly 126 bytes long. "
        "This way its length can't be encoded in seven bits of data.");
    const argdata_t *entries[] = {&argdata_null, str, &argdata_null};
    argdata_t *ad = argdata_create_seq(entries, __arraycount(entries));
    TEST_OBJECT(
        ad,
        "\x07\x80\x01\x80\x08This is a very long string whose length is "
        "exactly 126 bytes long. This way its length can't be encoded in "
        "seven bits of data.\x00\x80",
        0);
    argdata_free(ad);
  }
}

TEST(argdata_generate, str) {
  argdata_t *ad = argdata_create_str("Hello\x00world", 11);
  TEST_OBJECT(ad, "\x08Hello\x00world\x00", 0);
  argdata_free(ad);
}
