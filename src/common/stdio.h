// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_STDIO_H
#define COMMON_STDIO_H

#include <common/locale.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct fileops {
  bool (*read_peek)(FILE *);         // Obtain read buffer.
  bool (*write_flush)(FILE *);       // Flush write buffer.
  bool (*seek)(FILE *, off_t, int);  // Seek.
  bool (*setvbuf)(FILE *, size_t);   // Set buffer size.
};

struct __lockable _FILE {
  // Constant data.
  int fd;                        // Underlying file descriptor.
  int oflags;                    // Flags that were used to open the file.
  const struct fileops *ops;     // Underlying operations.
  const struct lc_ctype *ctype;  // Character set or wide character conversion.

  // Variable data.
  enum {
    F_EOF = 0x1,    // Stream is at end-of-file.
    F_ERROR = 0x2,  // An I/O error has occurred.
  } flags;
  int buftype;   // _IOFBF, _IOLBF or _IONBF.
  off_t offset;  // Negative if stream is not seekable.

  char *readbuf;
  size_t readbuflen;
  struct mbtoc32state readstate;

  char *writebuf;
  size_t writebuflen;

  // Data associated with this object.
  union {
    struct {
      char *buf;
      size_t used;
      size_t size;
    } fmemopen;
    struct {
      char *buf;
      size_t bufsize;
    } file;
    struct {
      char *readbuf;
      char *writebuf;
      size_t bufsize;
    } pipe;
    struct {
      char *buf;
      size_t used;
      size_t size;
      const char *owritebuf;
    } tmpfile;
  };

  // Per-stream lock. This must remain the last member of the object,
  // as fswap() will copy all structure members up to the lock.
  pthread_mutex_t lock;
#define FILE_COPYSIZE offsetof(FILE, lock)
};

static inline bool fseekable(FILE *stream) __requires_exclusive(*stream) {
  return stream->offset >= 0;
}

// Invocation of operations.

static inline bool fop_read_peek(FILE *stream) __requires_exclusive(*stream) {
  assert((stream->oflags & O_RDONLY) != 0 && "Stream not opened for reading");
  if (stream->ops->read_peek(stream)) {
    assert((!fseekable(stream) || stream->writebuflen == 0) &&
           "Write buffer still left intact");
    return true;
  }
  return false;
}

static inline bool fop_write_flush(FILE *stream) __requires_exclusive(*stream) {
  assert((stream->oflags & O_WRONLY) != 0 && "Stream not opened for writing");
  if (stream->ops->write_flush(stream)) {
    assert(stream->writebuflen > 0 &&
           "Write flushing did not yield a new write buffer");
    assert((!fseekable(stream) || stream->readbuflen == 0) &&
           "Read buffer still left intact");
    return true;
  }
  return false;
}

static inline bool fop_seek(FILE *stream, off_t offset, int whence)
    __requires_exclusive(*stream) {
  return stream->ops->seek(stream, offset, whence);
}

static inline bool fop_setvbuf(FILE *stream, size_t len)
    __requires_exclusive(*stream) {
  return stream->ops->setvbuf(stream, len);
}

// Locking functions.
//
// Implement these here, so these calls can be inlined into all of the
// stdio object files.

static inline void __flockfile(FILE *file)
    __locks_exclusive(*file) __no_lock_analysis {
  pthread_mutex_lock(&file->lock);
}

static inline int __ftrylockfile(FILE *file)
    __trylocks_exclusive(0, *file) __no_lock_analysis {
  return pthread_mutex_trylock(&(file)->lock) != 0;
}

static inline void __funlockfile(FILE *file)
    __unlocks(*file) __no_lock_analysis {
  pthread_mutex_unlock(&file->lock);
}

#define flockfile(file) __flockfile(file)
#define ftrylockfile(file) __ftrylockfile(file)
#define funlockfile(file) __funlockfile(file)

// Parses /[rwa]b?+?b?x?/.
static inline int get_oflags_from_string(const char *s) {
  int oflags;
  switch (*s++) {
    case 'r':
      oflags = O_RDONLY;
      break;
    case 'w':
      oflags = O_WRONLY | O_CREAT | O_TRUNC;
      break;
    case 'a':
      oflags = O_WRONLY | O_CREAT | O_APPEND;
      break;
    default:
      return 0;
  }
  if (*s == 'b')
    ++s;
  if (*s == '+') {
    oflags |= O_RDWR;
    ++s;
  }
  if (*s == 'b')
    ++s;
  if (*s == 'x') {
    oflags |= O_EXCL;
    ++s;
  }
  if (*s != '\0')
    return 0;
  return oflags;
}

static inline bool fread_peek(FILE *stream, const char **buf, size_t *buflen)
    __requires_exclusive(*stream) {
  // Refill the read buffer if empty.
  if (stream->readbuflen == 0) {
    // Only allow reads if we're opened for reading.
    if ((stream->oflags & O_RDONLY) == 0) {
      errno = EBADF;
      return false;
    }

    if (!fop_read_peek(stream)) {
      stream->flags |= F_ERROR;
      return false;
    }
    if (stream->readbuflen == 0)
      stream->flags |= F_EOF;
  }

  // Provide access to the read buffer.
  *buf = stream->readbuf;
  *buflen = stream->readbuflen;
  return true;
}

static inline void fread_consume(FILE *stream, size_t buflen)
    __requires_exclusive(*stream) {
  assert(buflen <= stream->readbuflen &&
         "Attempted to consume more data than available");
  stream->readbuf += buflen;
  stream->readbuflen -= buflen;
}

static inline size_t fwrite_put(FILE *stream, const char *buf, size_t inbuflen)
    __requires_exclusive(*stream) {
  // Only allow peeks if we're opened for writing.
  if ((stream->oflags & O_WRONLY) == 0) {
    errno = EBADF;
    return 0;
  }

  const char *inbuf = buf;
  while (inbuflen >= stream->writebuflen) {
    memcpy(stream->writebuf, inbuf, stream->writebuflen);
    inbuf += stream->writebuflen;
    inbuflen -= stream->writebuflen;
    stream->writebuf += stream->writebuflen;
    stream->writebuflen = 0;

    if (!fop_write_flush(stream)) {
      stream->flags |= F_ERROR;
      return inbuf - buf - stream->writebuflen;
    }
  }

  // TODO(ed): Honour buffering mechanism.
  memcpy(stream->writebuf, inbuf, inbuflen);
  inbuf += inbuflen;
  stream->writebuf += inbuflen;
  stream->writebuflen -= inbuflen;
  return inbuf - buf;
}

static inline off_t ftello_fast(FILE *stream) __requires_exclusive(*stream) {
  assert(fseekable(stream) && "Stream is not seekable");
  assert((stream->readbuflen == 0 || stream->writebuflen == 0) &&
         "Read and write buffer both used on a streamable file");
  off_t result =
      stream->offset - (off_t)stream->readbuflen - (off_t)stream->writebuflen;
  assert(result >= 0 && "Negative offset after buffer size correction");
  return result;
}

static inline int __putc_unlocked(int c, FILE *stream)
    __requires_exclusive(*stream) {
  // Write single byte of data.
  unsigned char ch = c;
  if (fwrite_put(stream, (const char *)&ch, 1) == 0)
    return EOF;
  return ch;
}

#define putc_unlocked(c, stream) __putc_unlocked(c, stream)

// Allocates a stream.
FILE *__falloc(const char *mode, locale_t locale);

#endif
