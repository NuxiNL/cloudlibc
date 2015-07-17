// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/mman.h>

#include <errno.h>
#include <stddef.h>
#include <testing.h>

TEST(mmap, bad_anon) {
  // The file descriptor must be -1 when using MAP_ANON.
  ASSERT_EQ(MAP_FAILED, mmap(NULL, 123, PROT_READ | PROT_WRITE,
                             MAP_ANON | MAP_PRIVATE, fd_tmp, 0));
  ASSERT_EQ(EINVAL, errno);
}
