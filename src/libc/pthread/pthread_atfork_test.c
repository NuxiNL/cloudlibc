// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <testing.h>
#include <unistd.h>

// Locks should also work properly in combination with pthread_atfork().
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Prepare handler should be called in reverse order.
static int prepare_val;

static void prepare1(void) {
  ASSERT_EQ(1, prepare_val);
  prepare_val = 2;
}

static void prepare2(void) {
  prepare_val = 1;
}

static void prepare3(void) __no_lock_analysis {
  pthread_mutex_lock(&lock);
}

// Parent handler should be called in forward order.
static int parent_val;

static void parent1(void) {
  ASSERT_EQ(2, prepare_val);
  parent_val = 1;
}

static void parent2(void) {
  ASSERT_EQ(1, parent_val);
  parent_val = 2;
}

static void parent3(void) __no_lock_analysis {
  pthread_mutex_unlock(&lock);
}

// Child handler should be called in forward order.
static int child_val = 0;

static void child1(void) {
  ASSERT_EQ(2, prepare_val);
  child_val = 1;
}

static void child2(void) {
  ASSERT_EQ(1, child_val);
  child_val = 2;
}

static void child3(void) __no_lock_analysis {
  pthread_mutex_unlock(&lock);
}

static void setup_handlers(void) {
  ASSERT_EQ(0, pthread_atfork(prepare1, parent1, child1));
  ASSERT_EQ(0, pthread_atfork(prepare2, parent2, child2));
  ASSERT_EQ(0, pthread_atfork(prepare3, parent3, child3));
}

TEST(pthread_atfork, example) {
  // Register three atfork handlers.
  pthread_once_t once_control = PTHREAD_ONCE_INIT;
  ASSERT_EQ(0, pthread_once(&once_control, setup_handlers));

  // Fork process.
  int fd;
  int ret = pdfork(&fd);
  if (ret == 0) {
    // Only prepare and child handlers should have been invoked.
    ASSERT_EQ(2, prepare_val);
    ASSERT_EQ(2, child_val);
    _Exit(0);
  }

  ASSERT_LE(0, ret);

  // Only prepare and parent handlers should have been invoked.
  ASSERT_EQ(2, prepare_val);
  ASSERT_EQ(2, parent_val);
  ASSERT_EQ(0, child_val);

  // Validate exit code of child.
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(0, close(fd));

  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(0, si.si_status);
}
