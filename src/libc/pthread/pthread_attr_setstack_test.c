// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <testing.h>

TEST(pthread_attr_setstack, einval) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  // Stack smaller than PTHREAD_STACK_MIN.
  char stack;
  ASSERT_EQ(EINVAL, pthread_attr_setstack(&attr, &stack, 0));
  ASSERT_EQ(EINVAL,
            pthread_attr_setstack(&attr, &stack, PTHREAD_STACK_MIN - 1));

  // NULL stack address is already used for automatically allocated stacks.
  ASSERT_EQ(EINVAL, pthread_attr_setstack(&attr, NULL, PTHREAD_STACK_MIN));
  ASSERT_EQ(0, pthread_attr_init(&attr));
}

#define STACKSIZE 131072

static void *check_bounds(void *arg) {
  // Check whether a stack-allocated variable is actually placed within
  // the stack allocated for this thread.
  char *stack = arg;
  ASSERT_GE(&stack, stack);
  ASSERT_LT(&stack, stack + STACKSIZE);
  return NULL;
}

TEST(pthread_attr_setstack, example) {
  // Spawn a thread with a custom stack.
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  char buf[STACKSIZE];
  ASSERT_EQ(0, pthread_attr_setstack(&attr, buf, sizeof(buf)));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, &attr, check_bounds, buf));
  ASSERT_EQ(0, pthread_attr_destroy(&attr));
  ASSERT_EQ(0, pthread_join(thread, NULL));
}
