// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/mman.h>
#include <sys/procdesc.h>

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <testing.h>
#include <unistd.h>

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
  atomic_uint serial_threads;
};

static void *do_wait(void *arg) {
  struct data *data = arg;

  // Wait on the barrier a hundred times. Register every time we return
  // PTHREAD_BARRIER_SERIAL_THREAD.
  for (int i = 0; i < 100; ++i) {
    int ret = pthread_barrier_wait(&data->barrier);
    ASSERT_TRUE(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);
    if (ret == PTHREAD_BARRIER_SERIAL_THREAD)
      atomic_fetch_add_explicit(&data->serial_threads, 1, memory_order_relaxed);
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
            pthread_barrier_init(&data.barrier, NULL, __arraycount(threads)));
  atomic_init(&data.serial_threads, 0);

  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(0, pthread_create(&threads[i], NULL, do_wait, &data));
  for (size_t i = 0; i < __arraycount(threads); ++i)
    ASSERT_EQ(0, pthread_join(threads[i], NULL));

  ASSERT_EQ(100, atomic_load(&data.serial_threads));
  ASSERT_EQ(0, pthread_barrier_destroy(&data.barrier));
}

TEST(pthread_barrier, shared) {
  // Same as above, but using a process-shared barrier.
  struct data *data = mmap(NULL, sizeof(*data), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, data);
  int fds[10];
  pthread_barrierattr_t attr;
  ASSERT_EQ(0, pthread_barrierattr_init(&attr));
  ASSERT_EQ(0, pthread_barrierattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));
  ASSERT_EQ(0, pthread_barrier_init(&data->barrier, &attr, __arraycount(fds)));
  ASSERT_EQ(0, pthread_barrierattr_destroy(&attr));
  atomic_init(&data->serial_threads, 0);

  for (size_t i = 0; i < __arraycount(fds); ++i) {
    int ret = pdfork(&fds[i]);
    if (ret == 0) {
      do_wait(data);
      _Exit(0);
    }
    ASSERT_LT(0, ret);
  }
  for (size_t i = 0; i < __arraycount(fds); ++i) {
    siginfo_t si;
    ASSERT_EQ(0, pdwait(fds[i], &si, 0));
    ASSERT_EQ(0, close(fds[i]));
  }

  ASSERT_EQ(100, atomic_load(&data->serial_threads));
  ASSERT_EQ(0, pthread_barrier_destroy(&data->barrier));
  ASSERT_EQ(0, munmap(data, sizeof(*data)));
}
