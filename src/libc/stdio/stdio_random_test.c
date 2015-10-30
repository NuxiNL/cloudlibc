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

// Creates a random string of a given length. It ensures that the string
// itself contains no null bytes.
static void random_string(char *buf, size_t len) {
  arc4random_buf(buf, len);
  buf[len] = '\0';
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

// Applies a random operations on a stream, while checking whether the
// return values of the calls are consistent with what the file looks
// like.
//
// This function is used to test streams that are created by functions
// such as fdopen(), tmpfile() and fmemopen() to see whether they behave
// correctly when presented a random sequence of operations.
static void apply_random_operations(FILE *stream) {
  char contents[RANDOM_BUFFER_SIZE] = {};
  off_t offset = 0, length = 0;
  bool has_eof = false;
  bool has_error = false;

  for (int i = 0; i < 1000; ++i) {
    switch (arc4random_uniform(22)) {
      case 0:
        // clearerr() should only clear the end-of-file and error flags.
        dprintf(1, "clearerr\n");
        clearerr(stream);
        has_eof = false;
        has_error = false;
        break;
      case 1:
        // feof() should only test the end-of-file flag.
        dprintf(1, "feof %d\n", has_eof);
        ASSERT_EQ(has_eof, feof(stream) != 0);
        break;
      case 2:
        // ferror() should only test the error flag.
        dprintf(1, "ferror\n");
        ASSERT_EQ(has_error, ferror(stream) != 0);
        break;
      case 3:
        // fflush() should have no observable effect.
        dprintf(1, "fflush\n");
        ASSERT_EQ(0, fflush(stream));
        break;
      case 4:
        // fgetc() should set the end-of-file flag when reading past the
        // file boundary.
        dprintf(1, "fgetc\n");
        if (offset >= length) {
          ASSERT_EQ(EOF, fgetc(stream));
          has_eof = true;
        } else {
          ASSERT_EQ((unsigned char)contents[offset++], fgetc(stream));
        }
        break;
      case 5: {
        // fgets().
        dprintf(1, "fgets\n");
        char readbuf[sizeof(contents)];
        size_t readlen = arc4random_uniform(sizeof(readbuf) + 1);
        if (readlen == 0) {
          // Call fgets() with buffer size zero. This behaviour isn't
          // standardized, but most implementations seem to return NULL.
          dprintf(1, "Zero read\n");
          ASSERT_EQ(NULL, fgets(readbuf, readlen, stream));
        } else if (offset >= length) {
          // TODO(ed): Pick a proper behaviour for this.
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
      case 6: {
        // fprintf(). Don't test formatting extensively. Just print a
        // randomly generated string using %s.
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
      case 7: {
        // fputc().
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
      case 8: {
        // fputs().
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
      case 9: {
        // fread().
        dprintf(1, "fread\n");
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
          ASSERT_ARREQ(contents + offset, readbuf, size * retitems);
          if (retitems != nitems) {
            // Short read at the end of the file.
            offset = length;
            has_eof = true;
          } else {
            // Read enclosed in the file.
            offset += size * nitems;
          }
        }
        break;
      }
      case 10:
        // fscanf().
        dprintf(1, "fscanf\n");
        // TODO(ed): Fix.
        break;
      case 11: {
        // fseeko(SEEK_CUR).
        dprintf(1, "fseeko(SEEK_CUR)\n");
        off_t new_offset =
            (off_t)arc4random_uniform(2 * sizeof(contents)) - sizeof(contents);
        if (new_offset < 0) {
          // Negative file offset.
          ASSERT_EQ(-1, fseeko(stream, new_offset - offset, SEEK_CUR));
          ASSERT_EQ(EINVAL, errno);
        } else {
          // Valid file offset.
          ASSERT_EQ(0, fseeko(stream, new_offset - offset, SEEK_CUR));
          offset = new_offset;
          has_eof = false;
        }
        break;
      }
      case 12: {
        // fseeko(SEEK_END).
        dprintf(1, "fseeko(SEEK_END)\n");
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
        }
        break;
      }
      case 13: {
        // fseeko(SEEK_SET).
        dprintf(1, "fseeko(SEEK_SET)\n");
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
        }
        break;
      }
      case 14:
        // ftello().
        dprintf(1, "ftello\n");
        ASSERT_EQ(offset, ftello(stream));
        break;
      case 15: {
        // fwrite().
        dprintf(1, "fwrite\n");
        size_t size, nitems;
        random_pair(sizeof(contents) - offset, &size, &nitems);
        char writebuf[sizeof(contents)];
        size_t writelen = size * nitems;
        arc4random_buf(writebuf, writelen);
        dprintf(1, "%ld %ld %zu %zu\n", offset, length, size, nitems);
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
      case 16:
        // getc() should set the end-of-file flag when reading past the
        // file boundary.
        dprintf(1, "getc\n");
        if (offset >= length) {
          ASSERT_EQ(EOF, getc(stream));
          has_eof = true;
        } else {
          ASSERT_EQ((unsigned char)contents[offset++], getc(stream));
        }
        break;
      case 17: {
        // getdelim().
        dprintf(1, "getdelim\n");
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
          const char *end = memchr(start, delim, remaining);
          size_t retval = end == NULL ? remaining : end - start + 1;
          ASSERT_EQ((ssize_t)retval, getdelim(&line, &linelen, delim, stream));
          ASSERT_ARREQ(start, line, retval);
          ASSERT_EQ('\0', line[retval]);
          offset += retval;
          // Set end-of-file if we returned the entire body, because the
          // matching character was not found.
          if (end == NULL)
            has_eof = true;
        }
        free(line);
        break;
      }
      case 18: {
        // getline().
        dprintf(1, "getline\n");
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
          const char *end = memchr(start, '\n', remaining);
          size_t retval = end == NULL ? remaining : end - start + 1;
          ASSERT_EQ((ssize_t)retval, getline(&line, &linelen, stream));
          ASSERT_ARREQ(start, line, retval);
          ASSERT_EQ('\0', line[retval]);
          offset += retval;
          // Set end-of-file if we returned the entire body, because the
          // matching character was not found.
          if (end == NULL)
            has_eof = true;
        }
        free(line);
        break;
      }
      case 19:
        // putc().
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
      case 20:
        // rewind() should reset the offset, the end-of-file flag and
        // the error flag.
        dprintf(1, "rewind\n");
        rewind(stream);
        offset = 0;
        has_eof = false;
        has_error = false;
        break;
      case 21:
        // ungetc().
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

TEST(stdio_random, fmemopen_buffer) {
  for (int i = 0; i < 100; ++i) {
    char contents[RANDOM_BUFFER_SIZE];
    FILE *stream = fmemopen(contents, sizeof(contents), "w+");
    apply_random_operations(stream);
  }
}

TEST(stdio_random, fmemopen_null) {
  for (int i = 0; i < 100; ++i) {
    FILE *stream = fmemopen(NULL, RANDOM_BUFFER_SIZE, "w+");
    apply_random_operations(stream);
  }
}

TEST(stdio_random, fopenat) {
  for (int i = 0; i < 100; ++i) {
    FILE *stream = fopenat(fd_tmp, "example", "w+");
    apply_random_operations(stream);
  }
}

TEST(stdio_random, tmpfile) {
  for (int i = 0; i < 100; ++i) {
    FILE *stream = tmpfile();
    apply_random_operations(stream);
  }
}
