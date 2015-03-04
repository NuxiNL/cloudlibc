// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/mman.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <testing.h>

TEST(msync, bad) {
  // MS_SYNC and MS_ASYNC are mutually exclusive.
  int x;
  ASSERT_EQ(-1, msync(&x, sizeof(x), 0));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(-1, msync(&x, sizeof(x), MS_ASYNC | MS_SYNC));
  ASSERT_EQ(EINVAL, errno);
}

TEST(msync, example) {
  // Create a file for testing and map it.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, ftruncate(fd, 100));
  void *addr = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  ASSERT_NE(MAP_FAILED, addr);

  // Lock file contents and perform a modification.
  ASSERT_EQ(0, mlock(addr, 100));
  *(char *)addr = 'A';

  // A regular sync should be successful.
  ASSERT_EQ(0, msync(addr, 100, MS_SYNC));

  // Invalidation should yield EBUSY, as it would discard the locked page.
  ASSERT_EQ(-1, msync(addr, 100, MS_SYNC | MS_INVALIDATE));
  ASSERT_EQ(EBUSY, errno);

  // Unlocking should allow us to invalidate the contents.
  ASSERT_EQ(0, munlock(addr, 100));
  ASSERT_EQ(0, msync(addr, 100, MS_SYNC | MS_INVALIDATE));

  ASSERT_EQ(0, munmap(addr, 100));
  ASSERT_EQ(0, close(fd));
}
