// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>
#include <testing.h>
#include <time.h>

TEST(pthread_mutex_timedlock, unlocked) __no_lock_analysis {
  // If the lock is unlocked, we should be able to acquire it, even if
  // the timestamp is already in the past.
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, NULL));
  ASSERT_EQ(0, pthread_mutex_timedlock(&mutex, &(struct timespec){}));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
  ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
}

static void *do_timedout(void *arg) {
  // Cannot lock it with a timestamp in the past.
  ASSERT_EQ(ETIMEDOUT, pthread_mutex_timedlock(arg, &(struct timespec){}));

  // And not with a timestamp in the near future.
  struct timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ++ts.tv_sec;
  ASSERT_EQ(ETIMEDOUT, pthread_mutex_timedlock(arg, &ts));
  return NULL;
}

TEST(pthread_mutex_timedlock, timedout) {
  // Spawn a thread that is unable to acquire the lock.
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, NULL));
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_timedout, &mutex));
  ASSERT_EQ(0, pthread_join(thread, NULL));

  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
  ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
}

static void *do_blocked(void *arg) __no_lock_analysis {
  // We should initially block on the lock, but after a short wait, we
  // should be able to acquire the lock.
  struct timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ++ts.tv_sec;
  ASSERT_EQ(0, pthread_mutex_timedlock(arg, &ts));
  ASSERT_EQ(0, pthread_mutex_unlock(arg));
  return NULL;
}

TEST(pthread_mutex_timedlock, blocked) {
  // Spawn a thread that is only initially unable to acquire the lock.
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, NULL));
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_blocked, &mutex));
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0,
                               &(struct timespec){.tv_nsec = 100000000L}));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
}
