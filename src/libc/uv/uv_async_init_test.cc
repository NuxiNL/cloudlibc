// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

static void close_cb(uv_handle_t *handle) {
}

static void foreground_cb(uv_async_t *async) {
  ASSERT_FALSE(*(bool *)async->data);
  *(bool *)async->data = true;
}

static void poke_async(uv_async_t *async) {
  for (int i = 0; i < 10000; ++i)
    ASSERT_EQ(0, uv_async_send(async));
}

TEST(uv_async_init, foreground) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Create an async object. It should not trigger by default.
  uv_async_t async;
  ASSERT_EQ(0, uv_async_init(&loop, &async, foreground_cb));
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_NOWAIT));

  // Wake up the async object. We should detect it immediately.
  poke_async(&async);
  bool called = false;
  async.data = &called;
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_NOWAIT));
  ASSERT_TRUE(called);

  uv_close((uv_handle_t *)&async, close_cb);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

static void poke_async_background(void *arg) {
  // Wait a bit to ensure the main thread has gone to sleep.
  usleep(10000);
  poke_async(static_cast<uv_async_t *>(arg));
}

TEST(uv_async_init, background) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Create an async object on which we perform a thousand calls from a
  // different thread.
  uv_async_t async;
  ASSERT_EQ(0, uv_async_init(&loop, &async, foreground_cb));
  uv_thread_t thread;
  ASSERT_EQ(0, uv_thread_create(&thread, poke_async_background, &async));

  // We should be able to detect this immediately.
  bool called = false;
  async.data = &called;
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_ONCE));
  ASSERT_TRUE(called);

  ASSERT_EQ(0, uv_thread_join(&thread));

  uv_close((uv_handle_t *)&async, close_cb);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}
