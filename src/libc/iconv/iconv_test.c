// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <iconv.h>
#include <testing.h>

TEST(iconv, example1) {
  iconv_t cd = iconv_open("ISO-8859-1", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "Kröller-Müller Museum";
  char out[5];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf;
  size_t outbytesleft;

  // Convert input string in groups of 5 bytes.
  outbuf = out;
  outbytesleft = sizeof(out);
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(E2BIG, errno);
  ASSERT_EQ(in + 6, inbuf);
  ASSERT_EQ(sizeof(in) - 7, inbytesleft);
  ASSERT_EQ(out + sizeof(out), outbuf);
  ASSERT_EQ(0, outbytesleft);
  ASSERT_ARREQ("Kr\xf6ll", out, __arraycount(out));

  outbuf = out;
  outbytesleft = sizeof(out);
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(E2BIG, errno);
  ASSERT_EQ(in + 12, inbuf);
  ASSERT_EQ(sizeof(in) - 13, inbytesleft);
  ASSERT_EQ(out + sizeof(out), outbuf);
  ASSERT_EQ(0, outbytesleft);
  ASSERT_ARREQ("er-M\xfc", out, __arraycount(out));

  outbuf = out;
  outbytesleft = sizeof(out);
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(E2BIG, errno);
  ASSERT_EQ(in + 17, inbuf);
  ASSERT_EQ(sizeof(in) - 18, inbytesleft);
  ASSERT_EQ(out + sizeof(out), outbuf);
  ASSERT_EQ(0, outbytesleft);
  ASSERT_ARREQ("ller ", out, __arraycount(out));

  outbuf = out;
  outbytesleft = sizeof(out);
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(E2BIG, errno);
  ASSERT_EQ(in + 22, inbuf);
  ASSERT_EQ(sizeof(in) - 23, inbytesleft);
  ASSERT_EQ(out + sizeof(out), outbuf);
  ASSERT_EQ(0, outbytesleft);
  ASSERT_ARREQ("Museu", out, __arraycount(out));

  outbuf = out;
  outbytesleft = sizeof(out);
  ASSERT_EQ(0, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(in + sizeof(in) - 1, inbuf);
  ASSERT_EQ(0, inbytesleft);
  ASSERT_EQ(out + 1, outbuf);
  ASSERT_EQ(sizeof(out) - 1, outbytesleft);
  ASSERT_ARREQ("m", out, 1);

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, example2) {
  iconv_t cd = iconv_open("euc-cn", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "The Netherlands: Нидерланды";
  char out[37];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf = out;
  size_t outbytesleft = sizeof(out);

  ASSERT_EQ(0, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(in + sizeof(in) - 1, inbuf);
  ASSERT_EQ(0, inbytesleft);
  ASSERT_EQ(out + sizeof(out), outbuf);
  ASSERT_EQ(0, outbytesleft);
  ASSERT_ARREQ(
      "The Netherlands: "
      "\xa7\xaf\xa7\xda\xa7\xd5\xa7\xd6\xa7\xe2\xa7\xdd\xa7\xd1\xa7\xdf\xa7\xd5"
      "\xa7\xed",
      out, __arraycount(out));

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, example3) {
  iconv_t cd = iconv_open("UTF-8", "EUC-CN");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] =
      "China: "
      "\xd6\xd0\xbb\xaa\xc8\xcb\xc3\xf1\xb9\xb2\xba\xcd\xb9\xfa";
  char out[28];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf = out;
  size_t outbytesleft = sizeof(out);

  ASSERT_EQ(0, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(in + sizeof(in) - 1, inbuf);
  ASSERT_EQ(0, inbytesleft);
  ASSERT_EQ(out + sizeof(out), outbuf);
  ASSERT_EQ(0, outbytesleft);
  ASSERT_ARREQ("China: 中华人民共和国", out, __arraycount(out));

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, bad_input) {
  iconv_t cd = iconv_open("ISO-8859-1", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "Broken\xffstring";
  char out[20];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf = out;
  size_t outbytesleft = sizeof(out);

  // Conversion should stop at \xff.
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(in + 6, inbuf);
  ASSERT_EQ(sizeof(in) - 7, inbytesleft);
  ASSERT_EQ(out + 6, outbuf);
  ASSERT_EQ(sizeof(out) - 6, outbytesleft);
  ASSERT_ARREQ("Broken", out, 6);

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, bad_input_ignore) {
  iconv_t cd = iconv_open("ISO-8859-1//IGNORE", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "Broken\xffstring";
  char out[20];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf = out;
  size_t outbytesleft = sizeof(out);

  // The \xff byte should be discarded.
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(in + sizeof(in) - 1, inbuf);
  ASSERT_EQ(0, inbytesleft);
  ASSERT_EQ(out + 12, outbuf);
  ASSERT_EQ(sizeof(out) - 12, outbytesleft);
  ASSERT_ARREQ("Brokenstring", out, 12);

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, bad_output) {
  iconv_t cd = iconv_open("ISO-8859-1", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "Hello☃World";
  char out[20];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf = out;
  size_t outbytesleft = sizeof(out);

  // Conversion should stop at the snowman.
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(in + 5, inbuf);
  ASSERT_EQ(sizeof(in) - 6, inbytesleft);
  ASSERT_EQ(out + 5, outbuf);
  ASSERT_EQ(sizeof(out) - 5, outbytesleft);
  ASSERT_ARREQ("Hello", out, 5);

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, bad_output_ignore) {
  iconv_t cd = iconv_open("ISO-8859-1//IGNORE", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "Hello☃World";
  char out[20];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf = out;
  size_t outbytesleft = sizeof(out);

  // The snowman should be discarded.
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(in + sizeof(in) - 1, inbuf);
  ASSERT_EQ(0, inbytesleft);
  ASSERT_EQ(out + 10, outbuf);
  ASSERT_EQ(sizeof(out) - 10, outbytesleft);
  ASSERT_ARREQ("HelloWorld", out, 10);

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, einval) {
  iconv_t cd = iconv_open("UTF-8", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "☃";
  char out[sizeof(in)];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = 1;
  char *outbuf = out;
  size_t outbytesleft = sizeof(out);

  // Parsing the character only partially
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(in, inbuf);
  ASSERT_EQ(1, inbytesleft);
  ASSERT_EQ(out, outbuf);
  ASSERT_EQ(sizeof(out), outbytesleft);

  ASSERT_EQ(0, iconv_close(cd));
}

TEST(iconv, partial_output) {
  iconv_t cd = iconv_open("UTF-8", "UTF-8");
  ASSERT_NE((iconv_t)-1, cd);

  // Buffers.
  static const char in[] = "☃";
  char out[10];

  // Pointers.
  char *inbuf = (char *)in;
  size_t inbytesleft = sizeof(in) - 1;
  char *outbuf;
  size_t outbytesleft;

  outbuf = out;
  outbytesleft = 1;
  ASSERT_EQ(-1, iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft));
  ASSERT_EQ(E2BIG, errno);
  ASSERT_EQ(in, inbuf);
  ASSERT_EQ(sizeof(in) - 1, inbytesleft);
  ASSERT_EQ(out, outbuf);
  ASSERT_EQ(1, outbytesleft);

  ASSERT_EQ(0, iconv_close(cd));
}
