// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fmtmsg.h>
#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(fmtmsg, bad) {
  // Replace stderr by a non-writable stream. fmtmsg() should fail.
  char buf;
  FILE *fp = fmemopen(&buf, 1, "r");
  ASSERT_NE(NULL, fp);
  fswap(fp, stderr);
  ASSERT_EQ(MM_NOTOK,
            fmtmsg(MM_PRINT, "Hello", MM_WARNING, "Text", "Action", "Tag"));
  fswap(fp, stderr);
  ASSERT_EQ(0, fclose(fp));
}

TEST(fmtmsg, example) {
#define TEST_FMTMSG(label, severity, text, action, tag, output)             \
  do {                                                                      \
    /* Replace stderr by a memory stream. */                                \
    char *buf;                                                              \
    size_t size;                                                            \
    FILE *fp = open_memstream(&buf, &size);                                 \
    ASSERT_NE(NULL, fp);                                                    \
    fswap(fp, stderr);                                                      \
                                                                            \
    /* Call fmtmsg() to write into the memory stream. */                    \
    ASSERT_EQ(MM_OK, fmtmsg(MM_PRINT, label, severity, text, action, tag)); \
    fswap(fp, stderr);                                                      \
    ASSERT_EQ(0, fclose(fp));                                               \
                                                                            \
    /* Compare memory stream contents against expected output. */           \
    ASSERT_STREQ(output, buf);                                              \
    free(buf);                                                              \
  } while (0)
  // Examples from POSIX.
  TEST_FMTMSG("XSI:cat", MM_ERROR, "illegal option",
              "refer to cat in user's reference manual", "XSI:cat:001",
              "XSI:cat: ERROR: illegal option\n"
              "TO FIX: refer to cat in user's reference manual XSI:cat:001\n");
  TEST_FMTMSG(MM_NULLLBL, MM_ERROR, "illegal option",
              "refer to cat in user's reference manual", MM_NULLTAG,
              "ERROR: illegal option\n"
              "TO FIX: refer to cat in user's reference manual\n");

  // No fields set.
  TEST_FMTMSG(MM_NULLLBL, MM_NULLSEV, MM_NULLTXT, MM_NULLACT, MM_NULLTAG, "");

  // Only a single field set.
  TEST_FMTMSG("Label", MM_NULLSEV, MM_NULLTXT, MM_NULLACT, MM_NULLTAG,
              "Label:\n");
  TEST_FMTMSG(MM_NULLLBL, MM_INFO, MM_NULLTXT, MM_NULLACT, MM_NULLTAG,
              "INFO:\n");
  TEST_FMTMSG(MM_NULLLBL, MM_NULLSEV, "Text", MM_NULLACT, MM_NULLTAG, "Text\n");
  TEST_FMTMSG(MM_NULLLBL, MM_NULLSEV, MM_NULLTXT, "Action", MM_NULLTAG,
              "TO FIX: Action\n");
  TEST_FMTMSG(MM_NULLLBL, MM_NULLSEV, MM_NULLTXT, MM_NULLACT, "Tag", "Tag\n");
#undef TEST_FMTMSG
}
