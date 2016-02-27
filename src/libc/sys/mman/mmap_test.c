// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/mman.h>

#include <errno.h>
#include <stddef.h>
#include <testing.h>

TEST(mmap, bad) {
  // The file descriptor must be -1 when using MAP_ANON.
  ASSERT_EQ(MAP_FAILED, mmap(NULL, 123, PROT_READ | PROT_WRITE,
                             MAP_ANON | MAP_PRIVATE, fd_tmp, 0));
  ASSERT_EQ(EINVAL, errno);

  // Invalid protection bits.
  ASSERT_EQ(MAP_FAILED,
            mmap(NULL, 123, 0xdeadc0de, MAP_ANON | MAP_PRIVATE, -1, 0));
  ASSERT_EQ(ENOTSUP, errno);
}

TEST(mmap, w_xor_x) {
  // Kernel should enforce W^X-like policy: PROT_WRITE | PROT_EXEC is
  // not allowed. ENOTSUP is the error code that should be returned if
  // the prot argument is not supported.
  ASSERT_EQ(MAP_FAILED, mmap(NULL, 123, PROT_READ | PROT_WRITE | PROT_EXEC,
                             MAP_ANON | MAP_PRIVATE, -1, 0));
  ASSERT_EQ(ENOTSUP, errno);

  // It should be allowed to toggle PROT_WRITE and PROT_EXEC.
  void *addr =
      mmap(NULL, 123, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  ASSERT_NE(MAP_FAILED, addr);
  ASSERT_EQ(-1, mprotect(addr, 123, PROT_READ | PROT_WRITE | PROT_EXEC));
  ASSERT_EQ(ENOTSUP, errno);
  ASSERT_EQ(0, mprotect(addr, 123, PROT_READ | PROT_EXEC));
  ASSERT_EQ(0, mprotect(addr, 123, PROT_READ | PROT_WRITE));

  ASSERT_EQ(0, munmap(addr, 123));
}
