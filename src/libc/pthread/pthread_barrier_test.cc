// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <iterator>

#include "gtest/gtest.h"

TEST(pthread_barrier, bad) {
  // Zero count is invalid.
  ASSERT_EQ(EINVAL, pthread_barrier_init(NULL, NULL, 0));
}

TEST(pthread_barrier, single) {
  // Only a single thread. Always return PTHREAD_BARRIER_SERIAL_THREAD.
  pthread_barrier_t barrier;
  ASSERT_EQ(0, pthread_barrier_init(&barrier, NULL, 1));

  ASSERT_EQ(PTHREAD_BARRIER_SERIAL_THREAD, pthread_barrier_wait(&barrier));
  ASSERT_EQ(PTHREAD_BARRIER_SERIAL_THREAD, pthread_barrier_wait(&barrier));
  ASSERT_EQ(PTHREAD_BARRIER_SERIAL_THREAD, pthread_barrier_wait(&barrier));

  ASSERT_EQ(0, pthread_barrier_destroy(&barrier));
}

struct data {
  pthread_barrier_t barrier;
  std::atomic<unsigned int> serial_threads;
};

static void *do_wait(void *arg) {
  auto data = static_cast<struct data *>(arg);

  // Wait on the barrier a hundred times. Register every time we return
  // PTHREAD_BARRIER_SERIAL_THREAD.
  for (int i = 0; i < 100; ++i) {
    int ret = pthread_barrier_wait(&data->barrier);
    EXPECT_TRUE(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);
    if (ret == PTHREAD_BARRIER_SERIAL_THREAD)
      data->serial_threads.fetch_add(1, std::memory_order_relaxed);
  }
  return NULL;
}

TEST(pthread_barrier, private) {
  // Create a threadpool of ten threads and let them wait on the barrier
  // a hundred times. In the end, we should have returned exactly
  // PTHREAD_BARRIER_SERIAL_THREAD 100 times.
  struct data data;
  pthread_t threads[10];
  ASSERT_EQ(0,
            pthread_barrier_init(&data.barrier, NULL, std::size(threads)));
  std::atomic_init(&data.serial_threads, 0U);

  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(0, pthread_create(&threads[i], NULL, do_wait, &data));
  for (size_t i = 0; i < std::size(threads); ++i)
    ASSERT_EQ(0, pthread_join(threads[i], NULL));

  ASSERT_EQ(100, data.serial_threads.load());
  ASSERT_EQ(0, pthread_barrier_destroy(&data.barrier));
}

#if 0  // TODO(ed): How to test this without forking?
TEST(pthread_barrier, shared) {
  // Same as above, but using a process-shared barrier.
  struct data *data = mmap(NULL, sizeof(*data), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, data);
  int fds[10];
  pthread_barrierattr_t attr;
  ASSERT_EQ(0, pthread_barrierattr_init(&attr));
  ASSERT_EQ(0, pthread_barrierattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));
  ASSERT_EQ(0, pthread_barrier_init(&data->barrier, &attr, std::size(fds)));
  ASSERT_EQ(0, pthread_barrierattr_destroy(&attr));
  atomic_init(&data->serial_threads, 0);

  for (size_t i = 0; i < std::size(fds); ++i) {
    int ret = pdfork(&fds[i]);
    if (ret == 0) {
      do_wait(data);
      _Exit(0);
    }
    ASSERT_LT(0, ret);
  }
  for (size_t i = 0; i < std::size(fds); ++i) {
    siginfo_t si;
    ASSERT_EQ(0, pdwait(fds[i], &si, 0));
    ASSERT_EQ(0, close(fds[i]));
  }

  ASSERT_EQ(100, atomic_load(&data->serial_threads));
  ASSERT_EQ(0, pthread_barrier_destroy(&data->barrier));
  ASSERT_EQ(0, munmap(data, sizeof(*data)));
}
#endif
