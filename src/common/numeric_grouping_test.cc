// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/numeric_grouping.h>

#include "gtest/gtest.h"

TEST(numeric_grouping, examples) {
#define NG_TEST(groupingstr, instr, outstr)                                \
  do {                                                                     \
    struct numeric_grouping ng;                                            \
    ASSERT_EQ(sizeof(outstr) - sizeof(instr),                              \
              numeric_grouping_init(&ng, (const signed char *)groupingstr, \
                                    sizeof(instr) - 1));                   \
    for (size_t i = 0, j = 0; i < sizeof(instr) - 1; ++i, ++j) {           \
      SCOPED_TRACE(i);                                                     \
      if (outstr[j] == ',') {                                              \
        ASSERT_TRUE(numeric_grouping_step(&ng));                           \
        ++j;                                                               \
      } else {                                                             \
        ASSERT_FALSE(numeric_grouping_step(&ng));                          \
      }                                                                    \
      ASSERT_EQ(outstr[j], instr[i]);                                      \
    }                                                                      \
  } while (0)
  NG_TEST(NULL, "", "");
  NG_TEST(NULL, "1234567", "1234567");
  NG_TEST("", "", "");
  NG_TEST("", "1234567", "1234567");

  NG_TEST("\x01", "1234567", "1,2,3,4,5,6,7");
  NG_TEST("\x02", "1234567", "1,23,45,67");
  NG_TEST("\x03", "1234567", "1,234,567");
  NG_TEST("\x04", "1234567", "123,4567");
  NG_TEST("\x05", "1234567", "12,34567");
  NG_TEST("\x06", "1234567", "1,234567");
  NG_TEST("\x07", "1234567", "1234567");
  NG_TEST("\x08", "1234567", "1234567");
  NG_TEST("\x03\x01\x02", "1234567", "1,23,4,567");
  NG_TEST("\x03\x01\x02", "123456789", "1,23,45,6,789");
  NG_TEST("\x03\xff", "1234567", "1234,567");
#undef NG_TEST
}
