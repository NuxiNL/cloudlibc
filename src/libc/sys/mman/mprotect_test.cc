// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <stddef.h>
#include <stdint.h>

#include "gtest/gtest.h"

#if 0  // TODO(ed): Come up with proper tests for failure.
TEST(mprotect, bad) {
  ASSERT_EQ(-1, mprotect((void *)UINTPTR_MAX, 100, PROT_READ | PROT_WRITE));
}
#endif

TEST(mprotect, example) {
  void *addr = mmap(NULL, 100, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, addr);

  // Make memory writable. Write value.
  ASSERT_EQ(0, mprotect(addr, 100, PROT_READ | PROT_WRITE));
  *(char *)addr = 'A';

  // Make memory readable. Check contents.
  ASSERT_EQ(0, mprotect(addr, 100, PROT_READ));
  ASSERT_EQ('A', *(char *)addr);

  ASSERT_EQ(0, munmap(addr, 100));
}
