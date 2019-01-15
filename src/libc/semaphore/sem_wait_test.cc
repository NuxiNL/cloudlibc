// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(sem_wait, noblock) {
  sem_t sem;
  ASSERT_EQ(0, sem_init(&sem, 0, 1));
  ASSERT_EQ(0, sem_wait(&sem));
  ASSERT_EQ(0, sem_destroy(&sem));
}

static void *do_post(void *arg) {
  struct timespec ts = {.tv_nsec = 100000000L};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
  EXPECT_EQ(0, sem_post(static_cast<sem_t *>(arg)));
  return NULL;
}

TEST(sem_wait, private) {
  // Spawn a thread that will post on the semaphore.
  sem_t sem;
  ASSERT_EQ(0, sem_init(&sem, 0, 0));
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_post, &sem));

  // Wait on the semaphore.
  ASSERT_EQ(0, sem_wait(&sem));

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(0, sem_destroy(&sem));
}

#if 0  // TODO(ed): How to test this without forking?
TEST(sem_wait, shared) {
  // Spawn a subprocess that will post on the semaphore.
  sem_t *sem = mmap(NULL, sizeof(*sem), PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANON, -1, 0);
  ASSERT_EQ(0, sem_init(sem, 1, 0));
  int fd;
  int ret = pdfork(&fd);
  if (ret == 0) {
    do_post(sem);
    _Exit(0);
  }
  ASSERT_LT(0, ret);

  // Wait on the semaphore.
  ASSERT_EQ(0, sem_wait(sem));

  ASSERT_EQ(0, close(fd));
  ASSERT_EQ(0, sem_destroy(sem));
  ASSERT_EQ(0, munmap(sem, sizeof(*sem)));
}
#endif
