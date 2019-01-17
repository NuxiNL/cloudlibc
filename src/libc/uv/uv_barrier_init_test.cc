// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <uv.h>
#include <atomic>

#include "gtest/gtest.h"

struct state {
  uv_barrier_t barrier;
  std::atomic<bool> got_serializer;
};

static void wait(void *arg) {
  auto state = static_cast<struct state *>(arg);
  int ret = uv_barrier_wait(&state->barrier);
  ASSERT_LE(0, ret);
  if (ret > 0)
    ASSERT_FALSE(state->got_serializer.exchange(true));
}

TEST(uv_barrier_init, example) {
  // Barriers with zero waiters are invalid.
  struct state state;
  ASSERT_EQ(UV_EINVAL, uv_barrier_init(&state.barrier, 0));

  // Create a barrier for ten waiters.
  ASSERT_EQ(0, uv_barrier_init(&state.barrier, 10));
  std::atomic_init(&state.got_serializer, false);

  // Spawn ten threads and let them wait on the barrier.
  uv_thread_t threads[10];
  for (unsigned int i = 0; i < 10; ++i)
    uv_thread_create(&threads[i], wait, &state);
  for (unsigned int i = 0; i < 10; ++i)
    uv_thread_join(&threads[i]);

  // One should have become the serializer.
  ASSERT_TRUE(state.got_serializer.load());
  uv_barrier_destroy(&state.barrier);
}
