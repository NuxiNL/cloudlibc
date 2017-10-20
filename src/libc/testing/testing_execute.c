// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>
#include <sys/select.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <fenv.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <testing.h>
#include <unistd.h>

#include "testing_impl.h"

// Markers for the beginning and end of the tests section of the
// executable. The empty_test array forces the creation of the section
// even if no tests have been registered.
extern struct __test __start___tests[0];
static struct __test empty_test[0] __section("__tests") __used;
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

static void check_leaked_file_descriptors(const fd_set *used_file_descriptors) {
  bool terminate = false;
  for (int fd = 0; fd < FD_SETSIZE; ++fd) {
    bool should_exist = FD_ISSET(fd, used_file_descriptors);
    struct stat sb;
    bool does_exist = fstat(fd, &sb) == 0;
    if (!should_exist && does_exist) {
      terminate = true;
      __testing_printf("File descriptor %d leaked by test\n", fd);
    } else if (should_exist && !does_exist) {
      terminate = true;
      __testing_printf("File descriptor %d destroyed by test\n", fd);
    }
  }
  if (terminate)
    abort();
}

struct testing_state {
  int tmpdir;                           // Temporary directory.
  _Atomic(struct __test *) test_start;  // First test to be executed.
  const fd_set *used_file_descriptors;  // File descriptor leaking check.
  bool run_single_threaded;             // Run tests that are single threaded.
};

static void run_test(struct testing_state *state, struct __test *test) {
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

static void *run_tests(void *arg) {
  struct testing_state *state = arg;
  for (;;) {
    struct __test *test =
        atomic_fetch_add_explicit(&state->test_start, 1, memory_order_relaxed);
    if (test >= __stop___tests)
      return NULL;

    if (test->__single_threaded == state->run_single_threaded) {
      run_test(state, test);
      if (state->used_file_descriptors != NULL)
        check_leaked_file_descriptors(state->used_file_descriptors);
    }
  }
}

void testing_execute(int tmpdir, int logfile, unsigned int nthreads) {
  // Shuffle all the tests on startup.
  static pthread_once_t shuffle_once = PTHREAD_ONCE_INIT;
  pthread_once(&shuffle_once, shuffle_tests);

  // Serialize execution of tests, as the logfile file descriptor needs
  // to be stored in a global variable.
  pthread_mutex_lock(&testing_lock);
  testing_logfile = logfile;

  // Record which file descriptors are in use for leak checking.
  fd_set used_file_descriptors;
  FD_ZERO(&used_file_descriptors);
  for (int fd = 0; fd < FD_SETSIZE; ++fd) {
    struct stat sb;
    if (fstat(fd, &sb) == 0)
      FD_SET(fd, &used_file_descriptors);
  }

  // Start off with tests that don't need to be run single threaded.
  struct testing_state state = {
      .tmpdir = tmpdir,
      .test_start = __start___tests,
      .run_single_threaded = false,
  };
  if (nthreads <= 1) {
    // Threading is disabled, so we can do used file descriptor checking
    // per test.
    state.used_file_descriptors = &used_file_descriptors;
    run_tests(&state);
  } else {
    // Spawn a number of threads to execute the tests in parallel,
    // meaning that we can only check for leaked file descriptors
    // afterwards.
    pthread_t threads[nthreads];
    for (size_t i = 0; i < nthreads; ++i)
      ASSERT_EQ(0, pthread_create(&threads[i], NULL, run_tests, &state));
    for (size_t i = 0; i < nthreads; ++i)
      ASSERT_EQ(0, pthread_join(threads[i], NULL));
    check_leaked_file_descriptors(&used_file_descriptors);
  }

  // Rewind and run all of the tests that require to be run single threaded.
  state.test_start = __start___tests;
  state.used_file_descriptors = &used_file_descriptors;
  state.run_single_threaded = true;
  run_tests(&state);

  __testing_printf("=> Successfully executed %zu tests\n",
                   __stop___tests - __start___tests);
  pthread_mutex_unlock(&testing_lock);
}
