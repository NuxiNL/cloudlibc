// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdio.h>

#include "gtest/gtest.h"

TEST(ftrylockfile, success) __no_lock_analysis {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Acquire and release the lock recursively.
  ASSERT_EQ(0, ftrylockfile(fp));
  ASSERT_EQ(0, ftrylockfile(fp));
  ASSERT_EQ(0, ftrylockfile(fp));
  funlockfile(fp);
  funlockfile(fp);
  funlockfile(fp);

  ASSERT_EQ(0, fclose(fp));
}

static void *trylock_fail(void *fp) {
  // Locking the stream will fail.
  EXPECT_NE(0, ftrylockfile(static_cast<FILE *>(fp)));
  return NULL;
}

TEST(ftrylockfile, failure) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Spawn a thread that will attempt to lock a stream. This will fail,
  // as this thread already holds a lock on the stream.
  flockfile(fp);
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, trylock_fail, fp));
  ASSERT_EQ(0, pthread_join(thread, NULL));
  funlockfile(fp);

  ASSERT_EQ(0, fclose(fp));
}
