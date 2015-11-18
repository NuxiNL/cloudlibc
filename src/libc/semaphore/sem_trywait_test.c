// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <semaphore.h>
#include <testing.h>

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
