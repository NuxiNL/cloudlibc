// Copyright (c) 2016-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <program.h>

#include "gtest/gtest.h"

TEST(program_getpid, format) {
  // Validate the format of the resulting UUID. As we're generating
  // random UUIDs, the version number should be set to four.
  const char *uuid = program_getpid();
#define ASSERT_XDIGIT(c) \
  ASSERT_TRUE(((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f'))
  ASSERT_XDIGIT(uuid[0]);
  ASSERT_XDIGIT(uuid[1]);
  ASSERT_XDIGIT(uuid[2]);
  ASSERT_XDIGIT(uuid[3]);
  ASSERT_XDIGIT(uuid[4]);
  ASSERT_XDIGIT(uuid[5]);
  ASSERT_XDIGIT(uuid[6]);
  ASSERT_XDIGIT(uuid[7]);
  ASSERT_EQ('-', uuid[8]);
  ASSERT_XDIGIT(uuid[9]);
  ASSERT_XDIGIT(uuid[10]);
  ASSERT_XDIGIT(uuid[11]);
  ASSERT_XDIGIT(uuid[12]);
  ASSERT_EQ('-', uuid[13]);
  ASSERT_EQ('4', uuid[14]);
  ASSERT_XDIGIT(uuid[15]);
  ASSERT_XDIGIT(uuid[16]);
  ASSERT_XDIGIT(uuid[17]);
  ASSERT_EQ('-', uuid[18]);
  ASSERT_TRUE(uuid[19] == '8' || uuid[19] == '9' || uuid[19] == 'a' ||
              uuid[19] == 'b');
  ASSERT_XDIGIT(uuid[20]);
  ASSERT_XDIGIT(uuid[21]);
  ASSERT_XDIGIT(uuid[22]);
  ASSERT_EQ('-', uuid[23]);
  ASSERT_XDIGIT(uuid[24]);
  ASSERT_XDIGIT(uuid[25]);
  ASSERT_XDIGIT(uuid[26]);
  ASSERT_XDIGIT(uuid[27]);
  ASSERT_XDIGIT(uuid[28]);
  ASSERT_XDIGIT(uuid[29]);
  ASSERT_XDIGIT(uuid[30]);
  ASSERT_XDIGIT(uuid[31]);
  ASSERT_XDIGIT(uuid[32]);
  ASSERT_XDIGIT(uuid[33]);
  ASSERT_XDIGIT(uuid[34]);
  ASSERT_XDIGIT(uuid[35]);
  ASSERT_EQ('\0', uuid[36]);
#undef ASSERT_XDIGIT
}

TEST(program_getpid, cached) {
  // Successive invocations should always return the same value.
  ASSERT_EQ(program_getpid(), program_getpid());
}
