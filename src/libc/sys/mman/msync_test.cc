// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(msync, bad) {
  // MS_SYNC and MS_ASYNC are mutually exclusive.
  int x;
  ASSERT_EQ(-1, msync(&x, sizeof(x), 0));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(-1, msync(&x, sizeof(x), MS_ASYNC | MS_SYNC));
  ASSERT_EQ(EINVAL, errno);
}

TEST(msync, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create a file for testing and map it.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, ftruncate(fd, 100));
  void *addr = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  ASSERT_NE(MAP_FAILED, addr);

  // TODO(ed): Is there anything meaningful we can test?
  ASSERT_EQ(0, msync(addr, 100, MS_SYNC));
  ASSERT_EQ(0, msync(addr, 100, MS_SYNC | MS_INVALIDATE));

  ASSERT_EQ(0, munmap(addr, 100));
  ASSERT_EQ(0, close(fd));
}
