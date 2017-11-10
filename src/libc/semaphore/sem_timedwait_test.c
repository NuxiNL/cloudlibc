// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <testing.h>
#include <time.h>

TEST(sem_timedwait, noblock) {
  sem_t sem;
  ASSERT_EQ(0, sem_init(&sem, 0, 1));
  ASSERT_EQ(0, sem_timedwait(&sem, &(struct timespec){.tv_sec = 1423663708}));
  ASSERT_EQ(0, sem_destroy(&sem));
}

TEST(sem_timedwait, timedout) {
  sem_t sem;
  ASSERT_EQ(0, sem_init(&sem, 0, 0));
  ASSERT_EQ(-1, sem_timedwait(&sem, &(struct timespec){.tv_sec = 1423663708}));
  ASSERT_EQ(ETIMEDOUT, errno);
  ASSERT_EQ(0, sem_destroy(&sem));
}

static void *do_post(void *arg) {
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0,
                               &(struct timespec){.tv_nsec = 100000000L}));
  ASSERT_EQ(0, sem_post(arg));
  return NULL;
}

TEST(sem_timedwait, success) {
  // Spawn a thread that will post on the semaphore.
  sem_t sem;
  ASSERT_EQ(0, sem_init(&sem, 0, 0));
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_post, &sem));

  // Wait on the semaphore for up to one second.
  struct timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ++ts.tv_sec;
  ASSERT_EQ(0, sem_timedwait(&sem, &ts));

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(0, sem_destroy(&sem));
}
