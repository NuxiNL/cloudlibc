// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

struct state {
  uv_mutex_t mutex;
  uv_cond_t cond;
};

static void wakeup_signal(void *arg) {
  auto state = static_cast<struct state *>(arg);
  uv_mutex_lock(&state->mutex);
  uv_cond_signal(&state->cond);
  uv_mutex_unlock(&state->mutex);
}

static void wakeup_broadcast(void *arg) {
  auto state = static_cast<struct state *>(arg);
  uv_mutex_lock(&state->mutex);
  uv_cond_broadcast(&state->cond);
  uv_mutex_unlock(&state->mutex);
}

TEST(uv_cond_init, example) {
  struct state state;
  ASSERT_EQ(0, uv_mutex_init(&state.mutex));
  ASSERT_EQ(0, uv_cond_init(&state.cond));

  // Wait indefinitely for the condition variable to trigger.
  uv_thread_t thread;
  uv_mutex_lock(&state.mutex);
  ASSERT_EQ(0, uv_thread_create(&thread, wakeup_signal, &state));
  uv_cond_wait(&state.cond, &state.mutex);
  uv_mutex_unlock(&state.mutex);
  uv_thread_join(&thread);

  // Wait up to a second.
  uv_mutex_lock(&state.mutex);
  ASSERT_EQ(0, uv_thread_create(&thread, wakeup_broadcast, &state));
  ASSERT_EQ(0, uv_cond_timedwait(&state.cond, &state.mutex, 1000000000));
  uv_mutex_unlock(&state.mutex);
  uv_thread_join(&thread);

  // Let the wait time out.
  uv_mutex_lock(&state.mutex);
  ASSERT_EQ(UV_ETIMEDOUT,
            uv_cond_timedwait(&state.cond, &state.mutex, 100000000));
  uv_mutex_unlock(&state.mutex);

  uv_cond_destroy(&state.cond);
  uv_mutex_destroy(&state.mutex);
}
