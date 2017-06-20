// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <stdio.h>
#include <testing.h>

TEST(setvbuf, iofbf) {
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

TEST(setvbuf, iolbf) {
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

TEST(setvbuf, ionbf) {
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
