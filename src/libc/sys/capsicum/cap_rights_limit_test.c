// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>
#include <sys/event.h>
#include <sys/stat.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(cap_rights_limit, bad) {
  // Bad file descriptor.
  ASSERT_EQ(-1, cap_rights_limit(-1, NULL));
  ASSERT_EQ(EBADF, errno);
}

TEST(cap_rights_limit, example) {
  int fd = kqueue();
  ASSERT_LE(0, fd);
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_TYPEISPOLL(sb.st_mode));

  // Attempted to extend rights.
  cap_rights_t rights;
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_READ));
  ASSERT_EQ(-1, cap_rights_limit(fd, &rights));
  ASSERT_EQ(ENOTCAPABLE, errno);
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_TYPEISPOLL(sb.st_mode));

  // Limiting rights is allowed.
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_KQUEUE));
  ASSERT_EQ(0, cap_rights_limit(fd, &rights));
  ASSERT_EQ(-1, fstat(fd, &sb));
  ASSERT_EQ(ENOTCAPABLE, errno);

  ASSERT_EQ(0, close(fd));
}
