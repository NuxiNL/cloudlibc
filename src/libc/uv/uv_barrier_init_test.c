// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <stdbool.h>
#include <testing.h>
#include <uv.h>

struct state {
  uv_barrier_t barrier;
  atomic_bool got_serializer;
};

static void wait(void *arg) {
  struct state *state = arg;
  int ret = uv_barrier_wait(&state->barrier);
  ASSERT_LE(0, ret);
  if (ret > 0)
    ASSERT_EQ(false, atomic_exchange(&state->got_serializer, true));
}

TEST(uv_barrier_init, example) {
  // Barriers with zero waiters are invalid.
  struct state state;
  ASSERT_EQ(UV_EINVAL, uv_barrier_init(&state.barrier, 0));

  // Create a barrier for ten waiters.
  ASSERT_EQ(0, uv_barrier_init(&state.barrier, 10));
  atomic_init(&state.got_serializer, false);

  // Spawn ten threads and let them wait on the barrier.
  uv_thread_t threads[10];
  for (unsigned int i = 0; i < 10; ++i)
    uv_thread_create(&threads[i], wait, &state);
  for (unsigned int i = 0; i < 10; ++i)
    uv_thread_join(&threads[i]);

  // One should have become the serializer.
  ASSERT_TRUE(atomic_load(&state.got_serializer));
  uv_barrier_destroy(&state.barrier);
}
