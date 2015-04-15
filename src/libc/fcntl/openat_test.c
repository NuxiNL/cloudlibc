// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(openat, o_append) {
  // Create a small test file.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_CREAT);
    ASSERT_LE(0, fd);
    ASSERT_EQ(5, write(fd, "Hello", 5));
    ASSERT_EQ(0, close(fd));
  }

  // Overwrite the data inside.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY);
    ASSERT_LE(0, fd);
    ASSERT_EQ(5, write(fd, "Hello", 5));
    ASSERT_EQ(5, lseek(fd, 0, SEEK_CUR));
    ASSERT_EQ(0, close(fd));
  }

  // Append data to the file.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_APPEND);
    ASSERT_LE(0, fd);
    ASSERT_EQ(5, write(fd, "Hello", 5));
    ASSERT_EQ(10, lseek(fd, 0, SEEK_CUR));
    ASSERT_EQ(0, close(fd));
  }
}

TEST(openat, o_creat) {
  // File does not exist.
  ASSERT_EQ(-1, openat(fd_tmp, "test", O_WRONLY));
  ASSERT_EQ(ENOENT, errno);

  // File has to be created explicitly.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_CREAT);
    ASSERT_LE(0, fd);
    ASSERT_EQ(0, close(fd));
  }
}

TEST(openat, o_directory) {
  // Create a file, a fifo and a directory for testing.
  {
    int fd = openat(fd_tmp, "file", O_WRONLY | O_CREAT);
    ASSERT_LE(0, fd);
    ASSERT_EQ(0, close(fd));
    ASSERT_EQ(0, mkfifoat(fd_tmp, "fifo"));
    ASSERT_EQ(0, mkdirat(fd_tmp, "dir"));
  }

  // Attempt to open them with O_DIRECTORY.
  ASSERT_EQ(-1, openat(fd_tmp, "file", O_RDONLY | O_DIRECTORY));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(-1, openat(fd_tmp, "fifo", O_RDONLY | O_DIRECTORY));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(-1, openat(fd_tmp, "dir", O_RDWR | O_DIRECTORY));
  ASSERT_EQ(EISDIR, errno);

  // Directory cannot be opened for writing.
  ASSERT_EQ(-1, openat(fd_tmp, "dir", O_RDWR | O_DIRECTORY));
  ASSERT_EQ(EISDIR, errno);

  {
    int fd = openat(fd_tmp, "dir", O_RDONLY | O_DIRECTORY);
    ASSERT_LE(0, fd);
    ASSERT_EQ(0, close(fd));
  }
}

TEST(openat, o_excl) {
  // Test file does not exist yet.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_CREAT | O_EXCL);
    ASSERT_LE(0, fd);
    ASSERT_EQ(0, close(fd));
  }

  // Second attempt should fail.
  ASSERT_EQ(-1, openat(fd_tmp, "test", O_WRONLY | O_CREAT | O_EXCL));
  ASSERT_EQ(EEXIST, errno);

  // Dropping O_EXCL should make it work again.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_CREAT);
    ASSERT_LE(0, fd);
    ASSERT_EQ(0, close(fd));
  }
}

TEST(openat, o_nonblock) {
  // Create a FIFO and open it for reading.
  ASSERT_EQ(0, mkfifoat(fd_tmp, "test"));
  int fd = openat(fd_tmp, "test", O_RDONLY | O_NONBLOCK);
  ASSERT_LE(0, fd);

  // Reading should fail.
  char buf[10];
  ASSERT_EQ(-1, read(fd, buf, sizeof(buf)));
  ASSERT_EQ(EAGAIN, errno);

  ASSERT_EQ(0, close(fd));

  // Cannot open a FIFO without a reader.
  ASSERT_EQ(0, mkfifoat(fd_tmp, "fifo"));
  ASSERT_EQ(-1, openat(fd_tmp, "fifo", O_WRONLY | O_NONBLOCK));
  ASSERT_EQ(ENXIO, errno);
}

TEST(openat, o_trunc) {
  // Create a small test file.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_CREAT);
    ASSERT_LE(0, fd);
    ASSERT_EQ(5, write(fd, "Hello", 5));
    ASSERT_EQ(0, close(fd));
  }

  // Opening without truncation should preseve file contents.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_CREAT);
    ASSERT_LE(0, fd);
    struct stat sb;
    ASSERT_EQ(0, fstat(fd, &sb));
    ASSERT_EQ(5, sb.st_size);
    ASSERT_EQ(0, close(fd));
  }

  // Truncate file contents.
  {
    int fd = openat(fd_tmp, "test", O_WRONLY | O_CREAT | O_TRUNC);
    ASSERT_LE(0, fd);
    struct stat sb;
    ASSERT_EQ(0, fstat(fd, &sb));
    ASSERT_EQ(0, sb.st_size);
    ASSERT_EQ(0, close(fd));
  }
}

TEST(openat, o_nofollow) {
  // Create a symbolic link for testing.
  ASSERT_EQ(0, symlinkat("to", fd_tmp, "from"));

  // Depending on whether O_NOFOLLOW is used, the error code should be
  // different.
  ASSERT_EQ(-1, openat(fd_tmp, "from", O_RDONLY | O_NOFOLLOW));
  ASSERT_EQ(ELOOP, errno);
  ASSERT_EQ(-1, openat(fd_tmp, "from", O_RDONLY));
  ASSERT_EQ(ENOENT, errno);
}
