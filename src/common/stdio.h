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
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct fileops {
  bool (*read_peek)(FILE *);          // Obtain read buffer.
  bool (*write_peek)(FILE *);         // Obtain write buffer.
  bool (*seek)(FILE *, off_t, bool);  // Seek.
  bool (*setvbuf)(FILE *, size_t);    // Set buffer size.
  bool (*flush)(FILE *);              // Flush read/write buffers.
  bool (*close)(FILE *);              // Free resources.
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
  int orientation;  // Wide-oriented or byte-oriented stream.
  int buftype;      // _IOFBF, _IOLBF or _IONBF.
  off_t offset;     // Negative if stream is not seekable.

  char ungetc[MB_LEN_MAX * 4];  // ungetc() buffer. Filled from the back.
  size_t ungetclen;             // Number of characters pushed back.

  char *readbuf;
  size_t readbuflen;
  struct mbtoc32state readstate;

  char *writebuf;
  size_t writebuflen;

  // Data associated with this object.
  union {
    struct {
      char *buf;
      const char *owritebuf;
      size_t used;
      size_t size;
      bool external;
    } fmemopen;
    struct {
      char *buf;
      char *written;
      size_t bufsize;
    } file;
    struct {
      char *readbuf;
      char *writebuf;
      char *written;
      size_t bufsize;
    } pipe;
    struct {
      char *buf;
      const char *owritebuf;
      size_t used;
      size_t size;
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
  assert(stream->readbuflen == 0 && "Reading while data is already present");
  assert(stream->ungetclen == 0 && "Reading while ungetc chars are present");
  if (stream->ops->read_peek(stream)) {
    assert((!fseekable(stream) || stream->writebuflen == 0) &&
           "Write buffer still left intact");
    return true;
  }
  return false;
}

static inline bool fop_write_peek(FILE *stream) __requires_exclusive(*stream) {
  assert((stream->oflags & O_WRONLY) != 0 && "Stream not opened for writing");
  if (stream->ops->write_peek(stream)) {
    assert(stream->writebuflen > 0 &&
           "Write flushing did not yield a new write buffer");
    assert((!fseekable(stream) || stream->readbuflen == 0) &&
           "Read buffer still left intact");
    return true;
  }
  return false;
}

static inline bool fop_seek(FILE *stream, off_t offset, bool seek_end)
    __requires_exclusive(*stream) {
  assert(fseekable(stream) && "Attempted to seek on an unseekable stream");
  return stream->ops->seek(stream, offset, seek_end);
}

static inline bool fop_setvbuf(FILE *stream, size_t len)
    __requires_exclusive(*stream) {
  assert(len > 0 && "Attempted to set buffer size to zero");
  return stream->ops->setvbuf(stream, len);
}

static inline bool fop_flush(FILE *stream) __requires_exclusive(*stream) {
  return stream->ops->flush(stream);
}

static inline bool fop_close(FILE *stream) {
  return stream->ops->close(stream);
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
  // First consume characters from the ungetc buffer.
  if (stream->ungetclen > 0) {
    *buf = stream->ungetc + sizeof(stream->ungetc) - stream->ungetclen;
    *buflen = stream->ungetclen;
    return true;
  }

  // Refill the read buffer if empty.
  if (stream->readbuflen == 0) {
    // Only allow reads if we're opened for reading.
    if ((stream->oflags & O_RDONLY) == 0) {
      stream->flags |= F_ERROR;
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
  if (stream->ungetclen > 0) {
    // Purge characters from the ungetc buffer.
    assert(buflen <= stream->ungetclen &&
           "Attempted to consume more data than available");
    stream->ungetclen -= buflen;
  } else {
    // Purge characters from the read buffer.
    assert(buflen <= stream->readbuflen &&
           "Attempted to consume more data than available");
    stream->readbuf += buflen;
    stream->readbuflen -= buflen;
  }
}

static inline int __getc_unlocked(FILE *stream) __requires_exclusive(*stream) {
  // Obtain the read buffer.
  const char *readbuf;
  size_t readbuflen;
  if (!fread_peek(stream, &readbuf, &readbuflen) || readbuflen == 0)
    return EOF;

  // Consume a single character.
  unsigned char ch = *readbuf;
  fread_consume(stream, 1);
  return ch;
}
#define getc_unlocked(stream) __getc_unlocked(stream)

static inline size_t fwrite_put(FILE *stream, const char *buf, size_t inbuflen)
    __requires_exclusive(*stream) {
  // Only allow peeks if we're opened for writing.
  if ((stream->oflags & O_WRONLY) == 0) {
    stream->flags |= F_ERROR;
    errno = EBADF;
    return 0;
  }

  const char *inbuf = buf;
  while (inbuflen > stream->writebuflen) {
    memcpy(stream->writebuf, inbuf, stream->writebuflen);
    inbuf += stream->writebuflen;
    inbuflen -= stream->writebuflen;
    stream->writebuf += stream->writebuflen;
    stream->writebuflen = 0;

    if (!fop_write_peek(stream)) {
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

static inline int __putc_unlocked(int c, FILE *stream)
    __requires_exclusive(*stream) {
  // Only allow storing characters if we're opened for writing.
  if ((stream->oflags & O_WRONLY) == 0) {
    stream->flags |= F_ERROR;
    errno = EBADF;
    return EOF;
  }

  if (stream->writebuflen == 0) {
    if (!fop_write_peek(stream)) {
      stream->flags |= F_ERROR;
      return EOF;
    }
  }

  // TODO(ed): Honour buffering mechanism.
  unsigned char ch = c;
  *stream->writebuf++ = ch;
  --stream->writebuflen;
  return ch;
}
#define putc_unlocked(c, stream) __putc_unlocked(c, stream)

// Offset within the underlying file of the stream object.
static inline off_t ftello_physical(FILE *stream)
    __requires_exclusive(*stream) {
  assert(fseekable(stream) && "Stream is not seekable");
  assert((stream->readbuflen == 0 || stream->writebuflen == 0) &&
         "Read and write buffer both used on a streamable file");
  off_t result =
      stream->offset - (off_t)stream->readbuflen - (off_t)stream->writebuflen;
  assert(result >= 0 && "Negative offset after buffer size correction");
  return result;
}

// Offset as reported by ftello(), namely accounting for the number of
// characters stored in the ungetc() buffer.
static inline off_t ftello_logical(FILE *stream) __requires_exclusive(*stream) {
  return ftello_physical(stream) - stream->ungetclen;
}

// Allocates a stream.
FILE *__falloc(const char *mode, locale_t locale);

#endif
