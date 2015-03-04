// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <threads.h>

static int do_timedout(void *arg) {
  // This should always fail, as the timestamp is in the past.
  struct timespec ts = {.tv_sec = 1422982367};
  ASSERT_EQ(thrd_timedout, mtx_timedlock(arg, &ts));
  return 0;
}

TEST(mtx_timedlock, timedout) {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_timed));
  ASSERT_EQ(thrd_success, mtx_lock(&mtx));

  thrd_t thrd;
  ASSERT_EQ(thrd_success, thrd_create(&thrd, do_timedout, &mtx));
  ASSERT_EQ(thrd_success, thrd_join(thrd, NULL));

  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));
  mtx_destroy(&mtx);
}

static int do_success(void *arg) __no_lock_analysis {
  // This should block until the lock becomes available.
  struct timespec ts;
  ASSERT_EQ(TIME_UTC, timespec_get(&ts, TIME_UTC));
  ++ts.tv_sec;
  ASSERT_EQ(thrd_success, mtx_timedlock(arg, &ts));
  ASSERT_EQ(thrd_success, mtx_unlock(arg));
  return 0;
}

TEST(mtx_timedlock, success) {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_timed));
  ASSERT_EQ(thrd_success, mtx_lock(&mtx));

  thrd_t thrd;
  ASSERT_EQ(thrd_success, thrd_create(&thrd, do_success, &mtx));

  // Unlock the mutex after waiting briefly.
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0,
                               &(struct timespec){.tv_nsec = 100000000}));
  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));

  ASSERT_EQ(thrd_success, thrd_join(thrd, NULL));
  mtx_destroy(&mtx);
}
