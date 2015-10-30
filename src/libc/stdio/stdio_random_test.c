// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <testing.h>

// Creates a random string of a fixed length.
static void random_string(char *buf, size_t len) {
  arc4random_buf(buf, len);
  buf[len] = '\0';

  // Set all zero bytes to one.
  for (size_t i = 0; i < len; ++i)
    if (buf[i] == 0)
      buf[i] = 1;
}

// Creates a pair of random numbers whose product does not exceed a
// maximum.
static void random_pair(size_t max, size_t *a, size_t *b) {
  size_t total = arc4random_uniform(max + 1);
  *a = arc4random_uniform(total + 1);
  *b = *a == 0 ? arc4random() : total / *a;
}

#define RANDOM_BUFFER_SIZE 1024

static void apply_random_operations(FILE *stream) {
  char contents[RANDOM_BUFFER_SIZE] = {};
  off_t offset = 0, length = 0;
  bool has_eof = false;
  bool has_error;

  // Apply a number of random operations on the file.
  for (int i = 0; i < 1000; ++i) {
    switch (arc4random_uniform(22)) {
      case 0:  // clearerr().
        dprintf(1, "clearerr\n");
        clearerr(stream);
        has_eof = false;
        has_error = false;
        break;
      case 1:  // feof().
        dprintf(1, "feof %d\n", has_eof);
        ASSERT_EQ(has_eof, feof(stream) != 0);
        break;
      case 2:  // ferror().
        dprintf(1, "ferror\n");
        ASSERT_EQ(has_error, ferror(stream) != 0);
        break;
      case 3:  // fflush().
        dprintf(1, "fflush\n");
        ASSERT_EQ(0, fflush(stream));
        break;
      case 4:  // fgetc().
        dprintf(1, "fgetc\n");
        if (offset >= length) {
          ASSERT_EQ(EOF, fgetc(stream));
          has_eof = true;
        } else {
          ASSERT_EQ((unsigned char)contents[offset], fgetc(stream));
          ++offset;
        }
        break;
      case 5: {  // fgets().
        dprintf(1, "fgets\n");
        char readbuf[sizeof(contents) * 2];
        size_t readlen = arc4random_uniform(sizeof(readbuf) + 1);
        if (readlen == 0) {
          dprintf(1, "Zero read\n");
          // XXX: We should return readbuf here?
          ASSERT_EQ(NULL, fgets(readbuf, readlen, stream));
        } else if (offset >= length) {
          dprintf(1, "Read past end of the file\n");
          if (fgets(readbuf, readlen, stream) != NULL) {
            ASSERT_EQ('\0', readbuf[0]);
          } else {
            has_eof = true;
          }
        } else {
          dprintf(1, "Read not past end of the file %ld + %zu, %ld\n", offset,
                  readlen, length);
          ASSERT_EQ(readbuf, fgets(readbuf, readlen, stream));
          size_t linelen = length - offset;
          if (linelen >= readlen)
            linelen = readlen - 1;
          const char *start = contents + offset;
          const char *end = memchr(start, '\n', linelen);
          if (end != NULL) {
            linelen = end - start + 1;
          }
          ASSERT_ARREQ(start, readbuf, linelen);
          ASSERT_EQ('\0', readbuf[linelen]);
          offset += linelen;

          // XXX: glibc sets the end-of-file indicator some times.
          clearerr(stream);
          has_eof = false;
          has_error = false;
        }
        break;
      }
      case 6: {  // fprintf().
        dprintf(1, "fprintf\n");
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
        break;
      }
      case 7: {  // fputc().
        dprintf(1, "fputc\n");
        if (offset < (off_t)sizeof(contents)) {
          unsigned int c;
          arc4random_buf(&c, sizeof(c));
          ASSERT_EQ((unsigned char)c, fputc(c, stream));
          contents[offset++] = c;
          if (length < offset)
            length = offset;
        }
        break;
      }
      case 8: {  // fputs().
        dprintf(1, "fputs\n");
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
        break;
      }
      case 9: {  // fread().
        dprintf(1, "fread\n");
        size_t size, nitems;
        random_pair(sizeof(contents) * 2, &size, &nitems);
        if (size == 0 || nitems == 0) {
          ASSERT_EQ(0, fread(NULL, 0, 0, stream));
        } else {
          if (offset >= length) {
            ASSERT_EQ(0, fread(NULL, size, nitems, stream));
            has_eof = true;
          } else {
            size_t retitems = (length - offset) / size;
            if (retitems > nitems)
              retitems = nitems;
            char readbuf[sizeof(contents)];
            ASSERT_EQ(retitems, fread(readbuf, size, nitems, stream));
            ASSERT_ARREQ(contents + offset, readbuf, size * retitems);
            if (retitems != nitems) {
              offset = length;
              has_eof = true;
            } else {
              offset += size * nitems;
            }
          }
        }
        break;
      }
      case 10:  // fscanf().
        // TODO(ed): Fix.
        break;
      case 11: {  // fseeko(SEEK_CUR).
        dprintf(1, "fseeko(SEEK_CUR)\n");
        off_t new_offset =
            (off_t)arc4random_uniform(2 * sizeof(contents)) - sizeof(contents);
        if (new_offset < 0) {
          ASSERT_EQ(-1, fseeko(stream, new_offset - offset, SEEK_CUR));
          ASSERT_EQ(EINVAL, errno);
        } else {
          ASSERT_EQ(0, fseeko(stream, new_offset - offset, SEEK_CUR));
          offset = new_offset;
          has_eof = false;
        }
        break;
      }
      case 12: {  // fseeko(SEEK_END).
        dprintf(1, "fseeko(SEEK_END)\n");
        off_t new_offset =
            (off_t)arc4random_uniform(2 * sizeof(contents)) - sizeof(contents);
        if (new_offset < 0) {
          ASSERT_EQ(-1, fseeko(stream, new_offset - length, SEEK_END));
          ASSERT_EQ(EINVAL, errno);
        } else {
          ASSERT_EQ(0, fseeko(stream, new_offset - length, SEEK_END));
          offset = new_offset;
          has_eof = false;
        }
        break;
      }
      case 13: {  // fseeko(SEEK_SET).
        dprintf(1, "fseeko(SEEK_SET)\n");
        off_t new_offset =
            (off_t)arc4random_uniform(2 * sizeof(contents)) - sizeof(contents);
        if (new_offset < 0) {
          ASSERT_EQ(-1, fseeko(stream, new_offset, SEEK_SET));
          ASSERT_EQ(EINVAL, errno);
        } else {
          ASSERT_EQ(0, fseeko(stream, new_offset, SEEK_SET));
          offset = new_offset;
          has_eof = false;
        }
        break;
      }
      case 14:  // ftello().
        dprintf(1, "ftello\n");
        ASSERT_EQ(offset, ftello(stream));
        break;
      case 15: {  // fwrite().
        dprintf(1, "fwrite\n");
        size_t size, nitems;
        random_pair(sizeof(contents) - offset, &size, &nitems);
        char writebuf[sizeof(contents)];
        size_t writelen = size * nitems;
        arc4random_buf(writebuf, writelen);
        dprintf(1, "%ld %zu %zu\n", offset, size, nitems);
        size_t retitems = writelen == 0 ? 0 : nitems;
        ASSERT_EQ(retitems, fwrite(writebuf, size, nitems, stream));
        if (writelen != 0) {
          memcpy(contents + offset, writebuf, writelen);
          offset += writelen;
          if (length < offset)
            length = offset;
        }
        break;
      }
      case 16:  // getc().
        dprintf(1, "getc\n");
        if (offset >= length) {
          ASSERT_EQ(EOF, fgetc(stream));
          has_eof = true;
        } else {
          ASSERT_EQ((unsigned char)contents[offset], getc(stream));
          ++offset;
        }
        break;
      case 17: {  // getdelim().
        dprintf(1, "getdelim\n");
        char *line = NULL;
        size_t linelen = 0;
        unsigned char delim;
        arc4random_buf(&delim, sizeof(delim));
        if (length <= offset) {
          ASSERT_EQ(-1, getdelim(&line, &linelen, delim, stream));
          has_eof = true;
        } else {
          size_t remaining = length - offset;
          const char *start = contents + offset;
          const char *end = memchr(start, delim, remaining);
          size_t retval = end == NULL ? remaining : end - start + 1;
          ASSERT_EQ((ssize_t)retval, getdelim(&line, &linelen, delim, stream));
          ASSERT_ARREQ(start, line, retval);
          ASSERT_EQ('\0', line[retval]);
          offset += retval;

          // TODO(ed): Is this all right?
          if (end == NULL)
            has_eof = true;
        }
        free(line);
        break;
      }
      case 18: {  // getline().
        dprintf(1, "getline\n");
        char *line = NULL;
        size_t linelen = 0;
        if (length <= offset) {
          ASSERT_EQ(-1, getline(&line, &linelen, stream));
          has_eof = true;
        } else {
          size_t remaining = length - offset;
          const char *start = contents + offset;
          const char *end = memchr(start, '\n', remaining);
          size_t retval = end == NULL ? remaining : end - start + 1;
          ASSERT_EQ((ssize_t)retval, getline(&line, &linelen, stream));
          ASSERT_ARREQ(start, line, retval);
          ASSERT_EQ('\0', line[retval]);
          offset += retval;

          // TODO(ed): Is this all right?
          if (end == NULL)
            has_eof = true;
        }
        free(line);
        break;
      }
      case 19:  // putc().
        dprintf(1, "putc\n");
        if (offset < (off_t)sizeof(contents)) {
          unsigned int c;
          arc4random_buf(&c, sizeof(c));
          ASSERT_EQ((unsigned char)c, putc(c, stream));
          contents[offset++] = c;
          if (length < offset)
            length = offset;
        }
        break;
      case 20:  // rewind().
        dprintf(1, "rewind\n");
        rewind(stream);
        offset = 0;
        has_eof = false;
        has_error = false;
        break;
      case 21:  // ungetc().
        dprintf(1, "ungetc\n");
        // TODO(ed): Fix.
        break;
    }
    ASSERT_LE(0, offset);
    ASSERT_GE((off_t)sizeof(contents), offset);
    ASSERT_LE(0, length);
    ASSERT_GE((off_t)sizeof(contents), length);
  }
  ASSERT_EQ(0, fclose(stream));
}

#if 0
TEST(stdio_random, fmemopen_buffer) {
  char contents[RANDOM_BUFFER_SIZE];
  FILE *stream = fmemopen(contents, sizeof(contents), "w+");
  apply_random_operations(stream);
}

TEST(stdio_random, fmemopen_null) {
  FILE *stream = fmemopen(NULL, RANDOM_BUFFER_SIZE, "w+");
  apply_random_operations(stream);
}

TEST(stdio_random, fopenat) {
  for (int i = 0; i < 100; ++i) {
    FILE *stream = fopenat(fd_tmp, "example", "w+");
    apply_random_operations(stream);
  }
}
#endif

TEST(stdio_random, tmpfile) {
  for (int i = 0; i < 1000; ++i) {
    FILE *stream = tmpfile();
    apply_random_operations(stream);
  }
}
