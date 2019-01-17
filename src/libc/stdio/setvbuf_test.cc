// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/ioctl.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(setvbuf, iofbf_file) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  FILE *f = fopenat(fd_tmp, "test", "w");
  ASSERT_NE(NULL, f);

  // Input should only become available after calling fflush().
  fprintf(f, "Hello");
  struct stat sb;
  ASSERT_EQ(0, fstat(fileno(f), &sb));
  ASSERT_EQ(0, sb.st_size);

  putc('\n', f);
  ASSERT_EQ(0, fstat(fileno(f), &sb));
  ASSERT_EQ(0, sb.st_size);

  fflush(f);
  ASSERT_EQ(0, fstat(fileno(f), &sb));
  ASSERT_EQ(6, sb.st_size);

  ASSERT_EQ(0, fclose(f));
}

TEST(setvbuf, iolbf_file) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  FILE *f = fopenat(fd_tmp, "test", "w");
  ASSERT_NE(NULL, f);
  ASSERT_EQ(0, setvbuf(f, NULL, _IOLBF, BUFSIZ));

  // Input should become visible after writing a newline.
  fprintf(f, "Hello");
  struct stat sb;
  ASSERT_EQ(0, fstat(fileno(f), &sb));
  ASSERT_EQ(0, sb.st_size);

  putc('\n', f);
  ASSERT_EQ(0, fstat(fileno(f), &sb));
  ASSERT_EQ(6, sb.st_size);

  ASSERT_EQ(0, fclose(f));
}

TEST(setvbuf, ionbf_file) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  FILE *f = fopenat(fd_tmp, "test", "w");
  ASSERT_NE(NULL, f);
  ASSERT_EQ(0, setvbuf(f, NULL, _IONBF, BUFSIZ));

  // Input should become visible immediately.
  fprintf(f, "Hello");
  struct stat sb;
  ASSERT_EQ(0, fstat(fileno(f), &sb));
  ASSERT_EQ(5, sb.st_size);

  ASSERT_EQ(0, fclose(f));
}

TEST(setvbuf, iofbf_pipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *f = fdopen(fds[1], "w");
  ASSERT_NE(NULL, f);

  // Input should only become available after calling fflush().
  fprintf(f, "Hello");
  int nbytes;
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &nbytes));
  ASSERT_EQ(0, nbytes);

  putc('\n', f);
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &nbytes));
  ASSERT_EQ(0, nbytes);

  fflush(f);
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &nbytes));
  ASSERT_EQ(6, nbytes);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, fclose(f));
}

TEST(setvbuf, iolbf_pipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *f = fdopen(fds[1], "w");
  ASSERT_NE(NULL, f);
  ASSERT_EQ(0, setvbuf(f, NULL, _IOLBF, BUFSIZ));

  // Input should become visible after writing a newline.
  fprintf(f, "Hello");
  int nbytes;
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &nbytes));
  ASSERT_EQ(0, nbytes);

  putc('\n', f);
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &nbytes));
  ASSERT_EQ(6, nbytes);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, fclose(f));
}

TEST(setvbuf, ionbf_pipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *f = fdopen(fds[1], "w");
  ASSERT_NE(NULL, f);
  ASSERT_EQ(0, setvbuf(f, NULL, _IONBF, BUFSIZ));

  // Input should become visible immediately.
  fprintf(f, "Hello");
  int nbytes;
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &nbytes));
  ASSERT_EQ(5, nbytes);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, fclose(f));
}
