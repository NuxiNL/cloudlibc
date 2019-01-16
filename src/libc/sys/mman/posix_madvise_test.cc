// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include "gtest/gtest.h"

TEST(posix_madvise, bad) {
  int x;
  ASSERT_EQ(EINVAL, posix_madvise(&x, sizeof(int), 0xdeadc0de));
  // TODO(ed): Determine what posix_madvise() should return here.
  ASSERT_NE(0, posix_madvise((void *)UINTPTR_MAX, 100, POSIX_MADV_NORMAL));
}

TEST(posix_madvise, example) {
  void *addr =
      mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, addr);
  ASSERT_EQ(0, posix_madvise(addr, 100, POSIX_MADV_RANDOM));
  ASSERT_EQ(0, munmap(addr, 100));
}
