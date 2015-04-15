// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>
#include <sys/event.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(kqueue, rights) {
  int fd = kqueue();
  ASSERT_LE(0, fd);

  // Validate file type.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_TYPEISPOLL(sb.st_mode));

  // Validate rights from a standard kqueue.
  cap_rights_t base, inheriting;
  ASSERT_EQ(0, cap_rights_get_explicit(fd, &base, &inheriting));
  ASSERT_EQ(CAP_FSTAT | CAP_KQUEUE, base.__value);
  ASSERT_EQ(0, inheriting.__value);

  // Validate access mode. This should return O_SEARCH, as it is the
  // best we can do. It is not readable, not writable and not executable.
  ASSERT_EQ(O_SEARCH, fcntl(fd, F_GETFL));

  ASSERT_EQ(0, close(fd));
}
