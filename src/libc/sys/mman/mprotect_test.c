// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/mman.h>

#include <stddef.h>
#include <stdint.h>
#include <testing.h>

TEST(mprotect, bad) {
  ASSERT_EQ(-1, mprotect((void *)UINTPTR_MAX, 100, PROT_READ | PROT_WRITE));
  // TODO(ed): Which error code should be returned?
}

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
