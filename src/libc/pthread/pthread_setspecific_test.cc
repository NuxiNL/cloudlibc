// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

#include "gtest/gtest.h"

struct params {
  pthread_key_t tss;
  int val;
};

static void dtor_setval(void *ptr) {
  *(int *)ptr = 1;
}

static void *do_dtor(void *arg) {
  auto params = static_cast<struct params *>(arg);
  EXPECT_EQ(NULL, pthread_getspecific(params->tss));

  // Set a bogus value.
  EXPECT_EQ(0, pthread_setspecific(params->tss, (void *)1));
  EXPECT_EQ((void *)1, pthread_getspecific(params->tss));

  // Point to an integer that will be set on teardown.
  EXPECT_EQ(0, pthread_setspecific(params->tss, &params->val));
  EXPECT_EQ(&params->val, pthread_getspecific(params->tss));
  return NULL;
}

TEST(pthread_setspecific, dtor) {
  struct params params = {};
  ASSERT_EQ(0, pthread_key_create(&params.tss, dtor_setval));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_dtor, &params));
  ASSERT_EQ(0, pthread_join(thread, NULL));

  // Destructor should have been called.
  ASSERT_EQ(1, params.val);
  ASSERT_EQ(0, pthread_key_delete(params.tss));
}

static void *do_clear(void *arg) {
  auto params = static_cast<struct params *>(arg);
  EXPECT_EQ(NULL, pthread_getspecific(params->tss));

  // Set a bogus value.
  EXPECT_EQ(0, pthread_setspecific(params->tss, (void *)1));
  EXPECT_EQ((void *)1, pthread_getspecific(params->tss));

  // Clear the value again so the destructor should not be called.
  EXPECT_EQ(0, pthread_setspecific(params->tss, NULL));
  EXPECT_EQ(NULL, pthread_getspecific(params->tss));
  return NULL;
}

TEST(pthread_setspecific, clear) {
  struct params params = {};
  ASSERT_EQ(0, pthread_key_create(&params.tss, dtor_setval));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_clear, &params));
  ASSERT_EQ(0, pthread_join(thread, NULL));

  // Destructor should not have been called.
  ASSERT_EQ(0, params.val);
  ASSERT_EQ(0, pthread_key_delete(params.tss));
}

static void *do_delete(void *arg) {
  auto params = static_cast<struct params *>(arg);
  EXPECT_EQ(NULL, pthread_getspecific(params->tss));

  // Set a bogus value.
  EXPECT_EQ(0, pthread_setspecific(params->tss, (void *)1));
  EXPECT_EQ((void *)1, pthread_getspecific(params->tss));

  // Delete the key so the destructor should not be called.
  EXPECT_EQ(0, pthread_key_delete(params->tss));
  return NULL;
}

TEST(pthread_setspecific, delete) {
  struct params params = {};
  ASSERT_EQ(0, pthread_key_create(&params.tss, dtor_setval));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_delete, &params));
  ASSERT_EQ(0, pthread_join(thread, NULL));

  // Destructor should not have been called.
  ASSERT_EQ(0, params.val);
}
