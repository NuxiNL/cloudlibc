// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/resource.h>

#include <errno.h>
#include <stddef.h>

#include "gtest/gtest.h"

TEST(getrusage, bad) {
  ASSERT_EQ(-1, getrusage(0xdeadc0de, NULL));
  ASSERT_EQ(EINVAL, errno);
}

TEST(getrusage, self) {
  // Perform bounds checking on time values.
  struct rusage ru;
  ASSERT_EQ(0, getrusage(RUSAGE_SELF, &ru));
  ASSERT_LE(0, ru.ru_utime.tv_sec);
  ASSERT_LE(0, ru.ru_utime.tv_usec);
  ASSERT_GT(1000000, ru.ru_utime.tv_usec);
  ASSERT_LE(0, ru.ru_stime.tv_sec);
  ASSERT_LE(0, ru.ru_stime.tv_usec);
  ASSERT_GT(1000000, ru.ru_stime.tv_usec);
}

TEST(getrusage, children) {
  // As we don't support a process hierarchy through traditional
  // forking, times for child processes should be zero.
  struct rusage ru;
  ASSERT_EQ(0, getrusage(RUSAGE_CHILDREN, &ru));
  ASSERT_EQ(0, ru.ru_utime.tv_sec);
  ASSERT_EQ(0, ru.ru_utime.tv_usec);
  ASSERT_EQ(0, ru.ru_stime.tv_sec);
  ASSERT_EQ(0, ru.ru_stime.tv_usec);
}
