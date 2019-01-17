// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <stddef.h>
#include <uv.h>

#include "gtest/gtest.h"

static void walk_cb_empty(uv_handle_t *handle, void *arg) {
  // The list should be empty.
  ASSERT_TRUE(false);
}

struct uv_walk_state {
  uv_prepare_t prepare;
  uv_idle_t idle;
  uv_timer_t timer;
  int step;
};

static void walk_cb_filled(uv_handle_t *handle, void *arg) {
  // Iteration over three elements.
  auto state = static_cast<struct uv_walk_state *>(arg);
  switch (state->step++) {
    case 0:
      ASSERT_EQ((uv_handle_t *)&state->prepare, handle);
      break;
    case 1:
      ASSERT_EQ((uv_handle_t *)&state->idle, handle);
      break;
    case 2:
      ASSERT_EQ((uv_handle_t *)&state->timer, handle);
      break;
    default:
      ASSERT_TRUE(false);
  }
}

static void close_cb(uv_handle_t *handle) {
}

TEST(uv_walk, example) {
  // Loop should initially be empty.
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));
  uv_walk(&loop, walk_cb_empty, NULL);

  // Insert three handles and walk over them.
  struct uv_walk_state state;
  ASSERT_EQ(0, uv_prepare_init(&loop, &state.prepare));
  ASSERT_EQ(0, uv_idle_init(&loop, &state.idle));
  ASSERT_EQ(0, uv_timer_init(&loop, &state.timer));
  state.step = 0;
  uv_walk(&loop, walk_cb_filled, &state);
  ASSERT_EQ(3, state.step);

  // Closing should not remove the handles immediately.
  uv_close((uv_handle_t *)&state.prepare, close_cb);
  uv_close((uv_handle_t *)&state.idle, close_cb);
  uv_close((uv_handle_t *)&state.timer, close_cb);
  state.step = 0;
  uv_walk(&loop, walk_cb_filled, &state);
  ASSERT_EQ(3, state.step);

  // Running the loop should cause them to be removed.
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  uv_walk(&loop, walk_cb_empty, NULL);
  ASSERT_EQ(0, uv_loop_close(&loop));
}
