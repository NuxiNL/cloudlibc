// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <threads.h>

#include "gtest/gtest.h"

struct params {
  tss_t tss;
  int val;
};

static void dtor_setval(void *ptr) {
  *(int *)ptr = 1;
}

static int do_dtor(void *arg) {
  auto params = static_cast<struct params *>(arg);
  EXPECT_EQ(NULL, tss_get(params->tss));

  // Set a bogus value.
  EXPECT_EQ(thrd_success, tss_set(params->tss, (void *)1));
  EXPECT_EQ((void *)1, tss_get(params->tss));

  // Point to an integer that will be set on teardown.
  EXPECT_EQ(thrd_success, tss_set(params->tss, &params->val));
  EXPECT_EQ(&params->val, tss_get(params->tss));
  return 0;
}

TEST(tss_set, dtor) {
  struct params params = {};
  ASSERT_EQ(thrd_success, tss_create(&params.tss, dtor_setval));

  thrd_t thread;
  ASSERT_EQ(thrd_success, thrd_create(&thread, do_dtor, &params));
  ASSERT_EQ(thrd_success, thrd_join(thread, NULL));

  // Destructor should have been called.
  ASSERT_EQ(1, params.val);
  tss_delete(params.tss);
}

static int do_clear(void *arg) {
  auto params = static_cast<struct params *>(arg);
  EXPECT_EQ(NULL, tss_get(params->tss));

  // Set a bogus value.
  EXPECT_EQ(thrd_success, tss_set(params->tss, (void *)1));
  EXPECT_EQ((void *)1, tss_get(params->tss));

  // Clear the value again so the destructor should not be called.
  EXPECT_EQ(thrd_success, tss_set(params->tss, NULL));
  EXPECT_EQ(NULL, tss_get(params->tss));
  return 0;
}

TEST(tss_set, clear) {
  struct params params = {};
  ASSERT_EQ(thrd_success, tss_create(&params.tss, dtor_setval));

  thrd_t thread;
  ASSERT_EQ(thrd_success, thrd_create(&thread, do_clear, &params));
  ASSERT_EQ(thrd_success, thrd_join(thread, NULL));

  // Destructor should not have been called.
  ASSERT_EQ(0, params.val);
  tss_delete(params.tss);
}

static int do_delete(void *arg) {
  auto params = static_cast<struct params *>(arg);
  EXPECT_EQ(NULL, tss_get(params->tss));

  // Set a bogus value.
  EXPECT_EQ(thrd_success, tss_set(params->tss, (void *)1));
  EXPECT_EQ((void *)1, tss_get(params->tss));

  // Delete the key so the destructor should not be called.
  tss_delete(params->tss);
  return 0;
}

TEST(tss_set, delete) {
  struct params params = {};
  ASSERT_EQ(thrd_success, tss_create(&params.tss, dtor_setval));

  thrd_t thread;
  ASSERT_EQ(thrd_success, thrd_create(&thread, do_delete, &params));
  ASSERT_EQ(thrd_success, thrd_join(thread, NULL));

  // Destructor should not have been called.
  ASSERT_EQ(0, params.val);
}
