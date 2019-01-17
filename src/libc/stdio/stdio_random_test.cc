// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

// Creates a random string of a given length. It ensures that the string
// itself contains no null bytes.
static void random_string(char *buf, size_t len) {
  arc4random_buf(buf, len);
  buf[len] = '\0';
  for (size_t i = 0; i < len; ++i)
    if (buf[i] == 0)
      buf[i] = 1;
}

// Creates a random wide character string containing just ISO-8859-1
// characters.
static void random_wstring(wchar_t *buf, size_t len) {
  unsigned char bytebuf[len];
  arc4random_buf(bytebuf, len);
  buf[len] = '\0';
  for (size_t i = 0; i < len; ++i)
    buf[i] = bytebuf[i] == 0 ? 1 : bytebuf[i];
}

// Creates a pair of random numbers whose product does not exceed a
// maximum.
static void random_pair(size_t max, size_t *a, size_t *b) {
  size_t total = arc4random_uniform(max + 1);
  *a = arc4random_uniform(total + 1);
  *b = *a == 0 ? arc4random() : total / *a;
}

// Copy wide characters into a byte buffer as ISO-8859-1.
static void to_narrow(char *out, const wchar_t *in, size_t n) {
  for (size_t i = 0; i < n; ++i)
    out[i] = in[i];
}

#define RANDOM_BUFFER_SIZE 1024

// Applies a random operations on a stream, while checking whether the
// return values of the calls are consistent with what the file looks
// like.
//
// This function is used to test streams that are created by functions
// such as fdopen(), tmpfile() and fmemopen() to see whether they behave
// correctly when presented a random sequence of operations.
//
// As this stdio implementation allows setvbuf() to be called at any
// time, also add some random invocations of this function. It should
// not have any impact that's externally visible.
//
// We also perform some simple testing for wide character functions, by
// using the ISO-8559-1 character set.
//
// TODO(ed): Make tests for fgets(), fread(), getdelim() and getline()
// work when characters have been pushed back using ungetc().
static void apply_random_operations(FILE *stream) {
  char contents[RANDOM_BUFFER_SIZE] = {};
  off_t offset = 0, length = 0;
  bool has_eof = false;
  bool has_error = false;
  char pushbacks[4];
  size_t npushbacks = 0;
  fpos_t position;
  off_t position_offset = -1;
  bool position_valid = false;
  int orientation = 0;

#define ORIENTATION(value)   \
  do {                       \
    if (orientation == 0)    \
      orientation = (value); \
  } while (0)
  for (int i = 0; i < 1000; ++i) {
    off_t logical_offset = offset - npushbacks;
    switch (arc4random_uniform(41)) {
      case 0:
        // clearerr() should only clear the end-of-file and error flags.
        clearerr(stream);
        has_eof = false;
        has_error = false;
        break;
      case 1:
        // feof() should only test the end-of-file flag.
        ASSERT_EQ(has_eof, feof(stream) != 0);
        break;
      case 2:
        // ferror() should only test the error flag.
        ASSERT_EQ(has_error, ferror(stream) != 0);
        break;
      case 3:
        // fflush() should have no observable effect.
        ASSERT_EQ(0, fflush(stream));
        npushbacks = 0;
        break;
      case 4:
        // fgetc() should set the end-of-file flag when reading past the
        // file boundary.
        if (npushbacks > 0) {
          // Should return character from ungetc().
          ASSERT_EQ((unsigned char)pushbacks[--npushbacks], fgetc(stream));
        } else if (offset < length) {
          ASSERT_EQ((unsigned char)contents[offset++], fgetc(stream));
        } else {
          ASSERT_EQ(EOF, fgetc(stream));
          has_eof = true;
        }
        ORIENTATION(-1);
        break;
      case 5:
        // fgetpos().
        if (logical_offset < 0) {
          ASSERT_EQ(-1, fgetpos(stream, &position));
          ASSERT_EQ(EOVERFLOW, errno);
        } else {
          ASSERT_EQ(0, fgetpos(stream, &position));
          position_valid = true;
          position_offset = logical_offset;
        }
        break;
      case 6: {
        // fgets().
        if (npushbacks == 0) {
          char readbuf[sizeof(contents)];
          size_t readlen = arc4random_uniform(sizeof(readbuf) + 1);
          if (readlen == 0) {
            // Call fgets() with buffer size zero. This behaviour isn't
            // standardized, but most implementations seem to return NULL.
            ASSERT_EQ(NULL, fgets(readbuf, readlen, stream));
          } else if (readlen == 1) {
            // This should always succeed by returning an empty buffer.
            ASSERT_EQ(readbuf, fgets(readbuf, readlen, stream));
            ASSERT_EQ('\0', readbuf[0]);
          } else if (offset >= length) {
            ASSERT_EQ(NULL, fgets(readbuf, readlen, stream));
            has_eof = true;
          } else {
            ASSERT_EQ(readbuf, fgets(readbuf, readlen--, stream));
            size_t linelen = length - offset;
            if (linelen > readlen)
              linelen = readlen;
            const char *start = contents + offset;
            auto end = static_cast<const char *>(memchr(start, '\n', linelen));
            if (end != NULL)
              linelen = end - start + 1;
            ASSERT_EQ(0, memcmp(start, readbuf, linelen));
            ASSERT_EQ('\0', readbuf[linelen]);
            // Set end-of-file if we returned the entire body, because the
            // matching character was not found.
            if (offset + (off_t)readlen > length && end == NULL)
              has_eof = true;
            offset += linelen;
          }
          ORIENTATION(-1);
        }
        break;
      }
      case 7:
        // fgetwc() should set the end-of-file flag when reading past
        // the file boundary.
        if (npushbacks > 0) {
          // Should return character from ungetwc().
          ASSERT_EQ((unsigned char)pushbacks[--npushbacks], fgetwc(stream));
        } else if (offset < length) {
          ASSERT_EQ((unsigned char)contents[offset++], fgetwc(stream));
        } else {
          ASSERT_EQ(WEOF, fgetwc(stream));
          has_eof = true;
        }
        ORIENTATION(1);
        break;
      case 8:
        // fgetws().
        if (npushbacks == 0) {
          wchar_t readbuf[sizeof(contents)];
          size_t readlen = arc4random_uniform(sizeof(readbuf) + 1);
          if (readlen == 0) {
            // Call fgets() with buffer size zero. This behaviour isn't
            // standardized, but most implementations seem to return NULL.
            ASSERT_EQ(NULL, fgetws(readbuf, readlen, stream));
          } else if (readlen == 1) {
            // This should always succeed by returning an empty buffer.
            ASSERT_EQ(readbuf, fgetws(readbuf, readlen, stream));
            ASSERT_EQ(L'\0', readbuf[0]);
          } else if (offset >= length) {
            ASSERT_EQ(NULL, fgetws(readbuf, readlen, stream));
            has_eof = true;
          } else {
            ASSERT_EQ(readbuf, fgetws(readbuf, readlen--, stream));
            size_t linelen = length - offset;
            if (linelen > readlen)
              linelen = readlen;
            const char *start = contents + offset;
            auto end = static_cast<const char *>(memchr(start, '\n', linelen));
            if (end != NULL)
              linelen = end - start + 1;
            for (size_t j = 0; j < linelen; ++j)
              ASSERT_EQ((unsigned char)start[j], readbuf[j]);
            ASSERT_EQ(L'\0', readbuf[linelen]);
            // Set end-of-file if we returned the entire body, because the
            // matching character was not found.
            if (offset + (off_t)readlen > length && end == NULL)
              has_eof = true;
            offset += linelen;
          }
          ORIENTATION(1);
        }
        break;
      case 9: {
        // fprintf(). Don't test formatting extensively. Just print a
        // randomly generated string using %s.
        size_t writelen = arc4random_uniform(sizeof(contents) - offset + 1);
        char writebuf[sizeof(contents) + 1];
        random_string(writebuf, writelen);
        ASSERT_EQ((int)writelen, fprintf(stream, "%s", writebuf));
        if (writelen != 0) {
          memcpy(contents + offset, writebuf, writelen);
          offset += writelen;
          if (length < offset)
            length = offset;
        }
        ORIENTATION(-1);
        break;
      }
      case 10: {
        // fputc().
        if (offset < (off_t)sizeof(contents)) {
          unsigned int c;
          arc4random_buf(&c, sizeof(c));
          ASSERT_EQ((unsigned char)c, fputc(c, stream));
          contents[offset++] = c;
          if (length < offset)
            length = offset;
          ORIENTATION(-1);
        }
        break;
      }
      case 11: {
        // fputs().
        size_t writelen = arc4random_uniform(sizeof(contents) - offset + 1);
        char writebuf[sizeof(contents) + 1];
        random_string(writebuf, writelen);
        ASSERT_LE(0, fputs(writebuf, stream));
        if (writelen != 0) {
          memcpy(contents + offset, writebuf, writelen);
          offset += writelen;
          if (length < offset)
            length = offset;
        }
        ORIENTATION(-1);
        break;
      }
      case 12:
        // fputwc().
        if (offset < (off_t)sizeof(contents)) {
          unsigned char c;
          arc4random_buf(&c, sizeof(c));
          ASSERT_EQ(c, fputwc(c, stream));
          contents[offset++] = c;
          if (length < offset)
            length = offset;
          ORIENTATION(1);
        }
      case 13: {
        // fputws().
        size_t writelen = arc4random_uniform(sizeof(contents) - offset + 1);
        wchar_t writebuf[sizeof(contents) + 1];
        random_wstring(writebuf, writelen);
        ASSERT_LE(0, fputws(writebuf, stream));
        if (writelen != 0) {
          to_narrow(contents + offset, writebuf, writelen);
          offset += writelen;
          if (length < offset)
            length = offset;
        }
        ORIENTATION(1);
        break;
      }
      case 14: {
        // fread().
        if (npushbacks == 0) {
          size_t size, nitems;
          random_pair(sizeof(contents) * 2, &size, &nitems);
          if (size == 0 || nitems == 0) {
            // Zero sized read should always return NULL and leave the
            // stream unaffected.
            ASSERT_EQ(0, fread(NULL, 0, 0, stream));
          } else if (offset >= length) {
            // Read past the end of the file.
            ASSERT_EQ(0, fread(NULL, size, nitems, stream));
            has_eof = true;
          } else {
            // Read in the body of the file.
            size_t retitems = (length - offset) / size;
            if (retitems > nitems)
              retitems = nitems;
            char readbuf[sizeof(contents)];
            ASSERT_EQ(retitems, fread(readbuf, size, nitems, stream));
            ASSERT_EQ(0, memcmp(contents + offset, readbuf, size * retitems));
            if (retitems != nitems) {
              // Short read at the end of the file.
              offset = length;
              has_eof = true;
            } else {
              // Read enclosed in the file.
              offset += size * nitems;
            }
          }
          ORIENTATION(-1);
        }
        break;
      }
      case 15:
        // fscanf().
        // TODO(ed): Fix.
        break;
      case 16: {
        // fseeko(SEEK_CUR).
        off_t new_offset =
            (off_t)arc4random_uniform(2 * sizeof(contents)) - sizeof(contents);
        if (new_offset < 0) {
          // Negative file offset.
          ASSERT_EQ(-1, fseeko(stream, new_offset - logical_offset, SEEK_CUR));
          ASSERT_EQ(EINVAL, errno);
        } else {
          // Valid file offset.
          ASSERT_EQ(0, fseeko(stream, new_offset - logical_offset, SEEK_CUR));
          offset = new_offset;
          has_eof = false;
          npushbacks = 0;
        }
        break;
      }
      case 17: {
        // fseeko(SEEK_END).
        off_t new_offset =
            (off_t)arc4random_uniform(2 * sizeof(contents)) - sizeof(contents);
        if (new_offset < 0) {
          // Negative file offset.
          ASSERT_EQ(-1, fseeko(stream, new_offset - length, SEEK_END));
          ASSERT_EQ(EINVAL, errno);
        } else {
          // Valid file offset.
          ASSERT_EQ(0, fseeko(stream, new_offset - length, SEEK_END));
          offset = new_offset;
          has_eof = false;
          npushbacks = 0;
        }
        break;
      }
      case 18: {
        // fseeko(SEEK_SET).
        off_t new_offset =
            (off_t)arc4random_uniform(2 * sizeof(contents)) - sizeof(contents);
        if (new_offset < 0) {
          // Negative file offset.
          ASSERT_EQ(-1, fseeko(stream, new_offset, SEEK_SET));
          ASSERT_EQ(EINVAL, errno);
        } else {
          // Valid file offset.
          ASSERT_EQ(0, fseeko(stream, new_offset, SEEK_SET));
          offset = new_offset;
          has_eof = false;
          npushbacks = 0;
        }
        break;
      }
      case 19:
        // fsetpos().
        if (position_offset >= 0) {
          ASSERT_EQ(0, fsetpos(stream, &position));
          offset = position_offset;
          has_eof = false;
          npushbacks = 0;
        }
        break;
      case 20:
        // ftello().
        ASSERT_EQ(logical_offset, ftello(stream));
        break;
      case 21:
        // fwide(-1).
        if (orientation == 0)
          orientation = -1;
        ASSERT_EQ(orientation, fwide(stream, -1));
        break;
      case 22:
        // fwide(0).
        ASSERT_EQ(orientation, fwide(stream, 0));
        break;
      case 23:
        // fwide(1).
        if (orientation == 0)
          orientation = 1;
        ASSERT_EQ(orientation, fwide(stream, 1));
        break;
      case 24: {
        // fwprintf(). Don't test formatting extensively. Just print a
        // randomly generated string using %ls.
        size_t writelen = arc4random_uniform(sizeof(contents) - offset + 1);
        wchar_t writebuf[sizeof(contents) + 1];
        random_wstring(writebuf, writelen);
        ASSERT_EQ((int)writelen, fwprintf(stream, L"%ls", writebuf));
        if (writelen != 0) {
          to_narrow(contents + offset, writebuf, writelen);
          offset += writelen;
          if (length < offset)
            length = offset;
        }
        ORIENTATION(1);
        break;
      }
      case 25:
        // fwscanf().
        // TODO(ed): Fix.
        break;
      case 26: {
        // fwrite().
        size_t size, nitems;
        random_pair(sizeof(contents) - offset, &size, &nitems);
        char writebuf[sizeof(contents)];
        size_t writelen = size * nitems;
        arc4random_buf(writebuf, writelen);
        size_t retitems = writelen == 0 ? 0 : nitems;
        ASSERT_EQ(retitems, fwrite(writebuf, size, nitems, stream));
        if (writelen != 0) {
          memcpy(contents + offset, writebuf, writelen);
          offset += writelen;
          if (length < offset)
            length = offset;
        }
        ORIENTATION(-1);
        break;
      }
      case 27:
        // getc() should set the end-of-file flag when reading past the
        // file boundary.
        if (npushbacks > 0) {
          // Should return character from ungetc().
          ASSERT_EQ((unsigned char)pushbacks[--npushbacks], getc(stream));
        } else if (offset < length) {
          ASSERT_EQ((unsigned char)contents[offset++], getc(stream));
        } else {
          ASSERT_EQ(EOF, getc(stream));
          has_eof = true;
        }
        ORIENTATION(-1);
        break;
      case 28: {
        // getdelim().
        if (npushbacks == 0) {
          char *line = NULL;
          size_t linelen = 0;
          unsigned char delim;
          arc4random_buf(&delim, sizeof(delim));
          if (offset >= length) {
            // Already at the end of the file.
            ASSERT_EQ(-1, getdelim(&line, &linelen, delim, stream));
            has_eof = true;
          } else {
            // Enclosed within the file contents.
            size_t remaining = length - offset;
            const char *start = contents + offset;
            auto end =
                static_cast<const char *>(memchr(start, delim, remaining));
            size_t retval = end == NULL ? remaining : end - start + 1;
            ASSERT_EQ((ssize_t)retval,
                      getdelim(&line, &linelen, delim, stream));
            ASSERT_EQ(0, memcmp(start, line, retval));
            ASSERT_EQ('\0', line[retval]);
            offset += retval;
            // Set end-of-file if we returned the entire body, because the
            // matching character was not found.
            if (end == NULL)
              has_eof = true;
          }
          free(line);
          ORIENTATION(-1);
          break;
        }
      }
      case 29: {
        // getline().
        if (npushbacks == 0) {
          char *line = NULL;
          size_t linelen = 0;
          if (offset >= length) {
            // Already at the end of the file.
            ASSERT_EQ(-1, getline(&line, &linelen, stream));
            has_eof = true;
          } else {
            // Enclosed within the file contents.
            size_t remaining = length - offset;
            const char *start = contents + offset;
            auto end =
                static_cast<const char *>(memchr(start, '\n', remaining));
            size_t retval = end == NULL ? remaining : end - start + 1;
            ASSERT_EQ((ssize_t)retval, getline(&line, &linelen, stream));
            ASSERT_EQ(0, memcmp(start, line, retval));
            ASSERT_EQ('\0', line[retval]);
            offset += retval;
            // Set end-of-file if we returned the entire body, because the
            // matching character was not found.
            if (end == NULL)
              has_eof = true;
          }
          ORIENTATION(-1);
          free(line);
          break;
        }
      }
      case 30:
        // getwc() should set the end-of-file flag when reading past the
        // file boundary.
        if (npushbacks > 0) {
          // Should return character from ungetwc().
          ASSERT_EQ((unsigned char)pushbacks[--npushbacks], getwc(stream));
        } else if (offset < length) {
          ASSERT_EQ((unsigned char)contents[offset++], getwc(stream));
        } else {
          ASSERT_EQ(WEOF, getwc(stream));
          has_eof = true;
        }
        ORIENTATION(1);
        break;
      case 31:
        // getwdelim().
        // TODO(ed): Fix.
        break;
      case 32:
        // getwline().
        // TODO(ed): Fix.
        break;
      case 33:
        // putc().
        if (offset < (off_t)sizeof(contents)) {
          unsigned int c;
          arc4random_buf(&c, sizeof(c));
          ASSERT_EQ((unsigned char)c, putc(c, stream));
          contents[offset++] = c;
          if (length < offset)
            length = offset;
          ORIENTATION(-1);
        }
        break;
      case 34:
        // putwc().
        if (offset < (off_t)sizeof(contents)) {
          unsigned char c;
          arc4random_buf(&c, sizeof(c));
          ASSERT_EQ(c, putwc(c, stream));
          contents[offset++] = c;
          if (length < offset)
            length = offset;
          ORIENTATION(1);
        }
        break;
      case 35:
        // rewind() should reset the offset, the end-of-file flag and
        // the error flag.
        rewind(stream);
        offset = 0;
        has_eof = false;
        has_error = false;
        npushbacks = 0;
        break;
      case 36:
        // setvbuf(_IOFBF).
        ASSERT_EQ(0, setvbuf(stream, NULL, _IOFBF,
                             arc4random_uniform(sizeof(contents) * 2)));
        break;
      case 37:
        // setvbuf(_IOLBF).
        ASSERT_EQ(0, setvbuf(stream, NULL, _IOLBF,
                             arc4random_uniform(sizeof(contents) * 2)));
        break;
      case 38:
        // setvbuf(_IONBF).
        ASSERT_EQ(0, setvbuf(stream, NULL, _IONBF,
                             arc4random_uniform(sizeof(contents) * 2)));
        break;
      case 39:
        // ungetc().
        if (npushbacks < sizeof(pushbacks)) {
          unsigned char ch;
          arc4random_buf(&ch, sizeof(ch));
          ASSERT_EQ(ch, ungetc(ch, stream));
          pushbacks[npushbacks++] = ch;
          has_eof = false;
          ORIENTATION(-1);
        }
        break;
      case 40:
        // ungetwc().
        if (npushbacks < sizeof(pushbacks)) {
          unsigned char ch;
          arc4random_buf(&ch, sizeof(ch));
          ASSERT_EQ(ch, ungetwc(ch, stream));
          pushbacks[npushbacks++] = ch;
          has_eof = false;
          ORIENTATION(1);
        }
        break;
    }
    ASSERT_LE(0, offset);
    ASSERT_GE((off_t)sizeof(contents), offset);
    ASSERT_LE(0, length);
    ASSERT_GE((off_t)sizeof(contents), length);
  }
#undef ORIENTATION
  ASSERT_EQ(0, fclose(stream));
}

TEST(stdio_random, fmemopen_buffer) {
  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  for (int i = 0; i < 100; ++i) {
    char contents[RANDOM_BUFFER_SIZE];
    FILE *stream = fmemopen_l(contents, sizeof(contents), "w+", locale);
    apply_random_operations(stream);
  }
  freelocale(locale);
}

TEST(stdio_random, fmemopen_null) {
  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  for (int i = 0; i < 100; ++i) {
    FILE *stream = fmemopen_l(NULL, RANDOM_BUFFER_SIZE, "w+", locale);
    apply_random_operations(stream);
  }
  freelocale(locale);
}

TEST(stdio_random, fopenat) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  for (int i = 0; i < 100; ++i) {
    FILE *stream = fopenat_l(fd_tmp, "example", "w+", locale);
    apply_random_operations(stream);
  }
  freelocale(locale);
}

TEST(stdio_random, tmpfile) {
  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  for (int i = 0; i < 100; ++i) {
    FILE *stream = tmpfile_l(locale);
    apply_random_operations(stream);
  }
  freelocale(locale);
}
