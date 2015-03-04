// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/mman.h>

#include <stddef.h>
#include <stdint.h>
#include <testing.h>

TEST(mlock, bad) {
  ASSERT_EQ(-1, mlock((void *)UINTPTR_MAX, 100));
  // TODO(edje): Which error code should be returned?
}

TEST(mlock, example) {
  void *addr =
      mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  ASSERT_NE(MAP_FAILED, addr);

  ASSERT_EQ(0, mlock(addr, 100));
  ASSERT_EQ(0, munlock(addr, 100));

  ASSERT_EQ(0, munmap(addr, 100));
}
