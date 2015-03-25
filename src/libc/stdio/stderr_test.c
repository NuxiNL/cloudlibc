// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdio.h>
#include <testing.h>

TEST(stderr, example) {
#if 0  // TODO(edje): This only works if stderr has not been retargeted.
  // stderr should not be seekable.
  ASSERT_EQ(-1, ftello(stderr));
  ASSERT_EQ(ESPIPE, errno);
  ASSERT_EQ(-1, fseeko(stderr, 0, SEEK_SET));
  ASSERT_EQ(ESPIPE, errno);

  // Output should be discarded.
  ASSERT_EQ(12, fprintf(stderr, "%s, %s", "Hello", "World"));
  ASSERT_EQ('!', putc('!', stderr));
  ASSERT_EQ(0, fflush(stderr));

  // Descriptor is not opened for reading.
  ASSERT_EQ(EOF, fgetc(stderr));
  ASSERT_EQ(EBADF, errno);
  int v;
  ASSERT_EQ(EOF, fscanf(stderr, "%d", &v));
  ASSERT_EQ(EBADF, errno);

  // Calls to setvbuf() should have no effect.
  ASSERT_EQ(0, setvbuf(stderr, NULL, _IOFBF, BUFSIZ));
  ASSERT_EQ(0, setvbuf(stderr, NULL, _IOLBF, BUFSIZ));
  ASSERT_EQ(0, setvbuf(stderr, NULL, _IONBF, BUFSIZ));
#endif
}
