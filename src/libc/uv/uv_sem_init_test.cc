// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_sem_init, example) {
  uv_sem_t sem;
  ASSERT_EQ(0, uv_sem_init(&sem, 2));

  // Perform try-waiting until the semaphore is exhausted.
  ASSERT_EQ(0, uv_sem_trywait(&sem));
  ASSERT_EQ(0, uv_sem_trywait(&sem));
  ASSERT_EQ(UV_EAGAIN, uv_sem_trywait(&sem));

  // Posting should increment the semaphore.
  uv_sem_post(&sem);
  ASSERT_EQ(0, uv_sem_trywait(&sem));
  ASSERT_EQ(UV_EAGAIN, uv_sem_trywait(&sem));

  // The blocking wait function should also decrement the semaphore.
  uv_sem_post(&sem);
  uv_sem_post(&sem);
  uv_sem_wait(&sem);
  uv_sem_wait(&sem);
  ASSERT_EQ(UV_EAGAIN, uv_sem_trywait(&sem));

  uv_sem_destroy(&sem);
}
