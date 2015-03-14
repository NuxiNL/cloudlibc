// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <fenv.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <testing.h>
#include <unistd.h>

#include "testing_impl.h"

extern struct __test __start___tests[0];
extern struct __test __stop___tests[0];

// Shuffle all the tests.
static void shuffle_tests(void) {
  for (ssize_t i = __stop___tests - __start___tests - 1; i > 0; --i) {
    size_t j = arc4random_uniform(i);
    struct __test tmp = __start___tests[i];
    __start___tests[i] = __start___tests[j];
    __start___tests[j] = tmp;
  }
}

// Lock for serializing tests output.
static pthread_mutex_t testing_lock = PTHREAD_MUTEX_INITIALIZER;
static int testing_logfile;

void __testing_printf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  vdprintf(testing_logfile, format, ap);
  va_end(ap);
}

struct testing_state {
  int tmpdir;                           // Temporary directory.
  _Atomic(struct __test *) test_start;  // First test to be executed.
};

static void *run_tests(void *arg) {
  struct testing_state *state = arg;
  for (;;) {
    struct __test *test =
        atomic_fetch_add_explicit(&state->test_start, 1, memory_order_relaxed);
    if (test >= __stop___tests)
      return NULL;

    __testing_printf("-> %s\n", test->__name);

    // Create a temporary directory for the test to store data.
    ASSERT_EQ(0, mkdirat(state->tmpdir, test->__name));
    int fd_tmp = openat(state->tmpdir, test->__name, O_DIRECTORY | O_RDONLY);
    ASSERT_NE(-1, fd_tmp);

    // Execute the test in a deterministic environment.
    errno = 0;
    fesetenv(FE_DFL_ENV);
    __test_note_stack = NULL;
    test->__func(fd_tmp);

    // Attempt to destroy the temporary directory to reduce used disk space.
    ASSERT_EQ(0, close(fd_tmp));
    unlinkat(state->tmpdir, test->__name, AT_REMOVEDIR);
  }
}

void testing_execute(int tmpdir, int logfile) {
  // Shuffle all the tests on startup.
  static pthread_once_t shuffle_once = PTHREAD_ONCE_INIT;
  pthread_once(&shuffle_once, shuffle_tests);

  // Serialize execution of tests, as the logfile file descriptor needs
  // to be stored in a global variable.
  pthread_mutex_lock(&testing_lock);
  testing_logfile = logfile;

  struct testing_state state = {
      .tmpdir = tmpdir, .test_start = __start___tests,
  };

  // Spawn a number of threads to execute the tests in parallel.
  // TODO(edje): Make number of threads configurable.
  pthread_t threads[8];
  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(0, pthread_create(&threads[i], NULL, run_tests, &state));
  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(0, pthread_join(threads[i], NULL));

  __testing_printf("=> Successfully executed %zu tests\n",
                   __stop___tests - __start___tests);
  pthread_mutex_unlock(&testing_lock);
}
