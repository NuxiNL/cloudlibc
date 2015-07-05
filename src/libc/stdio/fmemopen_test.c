// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdio.h>
#include <testing.h>

TEST(fmemopen, read) {
  // Construct a read-only memory file.
  static const char mytext[] = "Hello, world";
  FILE *file = fmemopen((char *)mytext, sizeof(mytext) - 1, "r");
  ASSERT_EQ(0, ftello(file));

  // We should not be able to write.
  ASSERT_EQ(0, fwrite(NULL, 12, 12, file));
  ASSERT_EQ(EBADF, errno);

  // Read the first six bytes using fread().
  char hello[6];
  ASSERT_EQ(6, fread(hello, 1, sizeof(hello), file));
  ASSERT_ARREQ("Hello,", hello, __arraycount(hello));
  ASSERT_EQ(6, ftello(file));
  ASSERT_FALSE(feof(file));

  // Read the rest of the string using fgets().
  char world[7];
  ASSERT_EQ(world, fgets(world, sizeof(world), file));
  ASSERT_STREQ(" world", world);
  ASSERT_EQ(12, ftello(file));
  ASSERT_FALSE(feof(file));

  // We should have reached EOF.
  ASSERT_EQ(0, fread(NULL, 1, 1, file));
  ASSERT_TRUE(feof(file));

  ASSERT_EQ(0, fclose(file));
}

TEST(fmemopen, append) {
  // Construct a partially finished memory file.
  char mytext[12] = "Hello,";
  FILE *file = fmemopen(mytext, sizeof(mytext), "a");
  ASSERT_EQ(6, ftello(file));

  // We should not be able to read.
  ASSERT_EQ(0, fread(NULL, 12, 12, file));
  ASSERT_EQ(EBADF, errno);

  // Append six more characters using fputs().
  ASSERT_EQ(0, fputs(" world", file));
  ASSERT_ARREQ("Hello, world", mytext, __arraycount(mytext));
  ASSERT_EQ(12, ftello(file));
  ASSERT_FALSE(ferror(file));

  // There should be no space left to write the final character.
  char ch = 'a';
  ASSERT_EQ(0, fwrite(&ch, 1, 1, file));
  ASSERT_TRUE(ferror(file));
  ASSERT_EQ(ENOSPC, errno);

  ASSERT_EQ(0, fclose(file));
}

// TODO(ed): Polish up code and add more tests.
