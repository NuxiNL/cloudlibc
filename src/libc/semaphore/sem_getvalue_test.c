// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <semaphore.h>
#include <testing.h>

TEST(sem_getvalue, example) {
  sem_t sem;
  int value;
  ASSERT_EQ(0, sem_init(&sem, 0, 12));
  ASSERT_EQ(0, sem_getvalue(&sem, &value));
  ASSERT_EQ(12, value);

  ASSERT_EQ(0, sem_wait(&sem));
  ASSERT_EQ(0, sem_getvalue(&sem, &value));
  ASSERT_EQ(11, value);
  ASSERT_EQ(0, sem_wait(&sem));
  ASSERT_EQ(0, sem_getvalue(&sem, &value));
  ASSERT_EQ(10, value);
  ASSERT_EQ(0, sem_wait(&sem));
  ASSERT_EQ(0, sem_getvalue(&sem, &value));
  ASSERT_EQ(9, value);

  ASSERT_EQ(0, sem_post(&sem));
  ASSERT_EQ(0, sem_getvalue(&sem, &value));
  ASSERT_EQ(10, value);
  ASSERT_EQ(0, sem_post(&sem));
  ASSERT_EQ(0, sem_getvalue(&sem, &value));
  ASSERT_EQ(11, value);
  ASSERT_EQ(0, sem_post(&sem));
  ASSERT_EQ(0, sem_getvalue(&sem, &value));
  ASSERT_EQ(12, value);

  ASSERT_EQ(0, sem_destroy(&sem));
}
