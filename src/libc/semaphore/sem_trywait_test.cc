// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <semaphore.h>

#include "gtest/gtest.h"

TEST(sem_trywait, failure) {
  sem_t sem;
  ASSERT_EQ(0, sem_init(&sem, 0, 0));
  ASSERT_EQ(-1, sem_trywait(&sem));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(0, sem_destroy(&sem));
}

TEST(sem_trywait, success) {
  sem_t sem;
  ASSERT_EQ(0, sem_init(&sem, 0, 1));
  ASSERT_EQ(0, sem_trywait(&sem));
  ASSERT_EQ(0, sem_destroy(&sem));
}
