// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <threads.h>

#include "gtest/gtest.h"

TEST(mtx_trylock, plain) __no_lock_analysis {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_plain));

  ASSERT_EQ(thrd_success, mtx_trylock(&mtx));
  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));

  mtx_destroy(&mtx);
}

TEST(mtx_trylock, recursive) __no_lock_analysis {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_plain | mtx_recursive));

  ASSERT_EQ(thrd_success, mtx_trylock(&mtx));
  ASSERT_EQ(thrd_success, mtx_trylock(&mtx));
  ASSERT_EQ(thrd_success, mtx_trylock(&mtx));
  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));
  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));
  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));

  mtx_destroy(&mtx);
}

static int busy(void *arg) {
  auto mtx = static_cast<mtx_t *>(arg);
  EXPECT_EQ(thrd_busy, mtx_trylock(mtx));
  return 0;
}

TEST(mtx_trylock, busy) __no_lock_analysis {
  mtx_t mtx;
  ASSERT_EQ(thrd_success, mtx_init(&mtx, mtx_plain));
  ASSERT_EQ(thrd_success, mtx_trylock(&mtx));

  // The thread should not be able to acquire the lock.
  thrd_t thrd;
  ASSERT_EQ(thrd_success, thrd_create(&thrd, busy, &mtx));
  ASSERT_EQ(thrd_success, thrd_join(thrd, NULL));

  ASSERT_EQ(thrd_success, mtx_unlock(&mtx));
  mtx_destroy(&mtx);
}
