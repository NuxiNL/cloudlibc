// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

#include "gtest/gtest.h"

static void prepare(void) {
}

static void parent(void) {
}

static void child(void) {
}

TEST(pthread_atfork, example) {
  // These handlers aren't called by this implementation, but they can
  // at least be registered.
  ASSERT_EQ(0, pthread_atfork(prepare, parent, child));
}
