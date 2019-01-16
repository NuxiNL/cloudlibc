// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(shm_open, bad) {
  ASSERT_EQ(-1, shm_open("/somepath", O_RDWR));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(-1, shm_open(SHM_ANON, O_RDONLY));
  ASSERT_EQ(EINVAL, errno);
}

TEST(shm_open, example) {
  int fd = shm_open(SHM_ANON, O_RDWR);
  ASSERT_LE(0, fd);

  // Shared memory should be empty.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_TYPEISSHM(&sb));
  ASSERT_EQ(0, sb.st_size);

  // Grow.
  ASSERT_EQ(0, ftruncate(fd, 1000));

  // Validate the length. It should be at least 1000, but some
  // implementations round it up to the nearest page boundary.
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_TYPEISSHM(&sb));
  ASSERT_LE(1000, sb.st_size);
  ASSERT_GE(__roundup(1000, sysconf(_SC_PAGESIZE)), sb.st_size);

  // Map shared memory and write data into it.
  void *mapping = mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  ASSERT_NE(MAP_FAILED, mapping);
  memset(mapping, 'A', 1000);
  ASSERT_EQ(0, munmap(mapping, 1000));

  ASSERT_EQ(0, close(fd));
}

TEST(shm_open, rights) {
  int fd = shm_open(SHM_ANON, O_RDWR);
  ASSERT_LE(0, fd);

  // Validate rights from a standard shared memory object.
  cap_rights_t base, inheriting;
  ASSERT_EQ(0, cap_rights_get_explicit(fd, &base, &inheriting));
  ASSERT_EQ(CAP_FSTAT | CAP_FTRUNCATE | CAP_MMAP_RWX, base.__value);
  ASSERT_EQ(0, inheriting.__value);

  ASSERT_EQ(0, close(fd));
}
