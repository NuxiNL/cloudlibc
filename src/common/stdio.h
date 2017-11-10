// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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

// Routines that need to be provided by a stream provider.
struct fileops {
  bool (*read_peek)(FILE *);          // Obtain read buffer.
  bool (*write_peek)(FILE *);         // Obtain write buffer.
  bool (*seek)(FILE *, off_t, bool);  // Seek.
  bool (*setvbuf)(FILE *, size_t);    // Set buffer size.
  bool (*flush)(FILE *);              // Flush read/write buffers.
  bool (*close)(FILE *);              // Free resources.
};

// Replacement for read_peek()/write_peek() that can be used if the
// operation conflicts with the access mode.
static inline bool ebadf(FILE *file) {
  errno = EBADF;
  return false;
}

// Replacement for seek() that can be used if the stream is not
// seekable. Setting this to NULL does not harm, as the seek() operation
// is never called if the offset is set to -1.
#define espipe NULL

// Macros for declaring operations tables for simple stream providers
// that do not distinguish between access modes.
#define DECLARE_FILEOPS_SIMPLE(prefix)                        \
  static struct fileops prefix##_ops = {                      \
      prefix##_read_peek, prefix##_write_peek, prefix##_seek, \
      prefix##_setvbuf,   prefix##_flush,      prefix##_close}
#define GET_FILEOPS_SIMPLE(prefix) (&prefix##_ops)

// Macros for declaring operations tables for stream providers that do
// distinguish between access modes.
#define DECLARE_FILEOPS_ACCMODE(prefix)                             \
  static struct fileops prefix##_ops[3] = {                         \
      {prefix##_read_peek, ebadf, prefix##_seek, prefix##_setvbuf,  \
       prefix##_flush, prefix##_close},                             \
      {ebadf, prefix##_write_peek, prefix##_seek, prefix##_setvbuf, \
       prefix##_flush, prefix##_close},                             \
      {prefix##_read_peek, prefix##_write_peek, prefix##_seek,      \
       prefix##_setvbuf, prefix##_flush, prefix##_close}}
#define GET_FILEOPS_ACCMODE(prefix, oflags) \
  (((oflags)&O_WRONLY) == 0                 \
       ? &prefix##_ops[0]                   \
       : ((oflags)&O_RDONLY) == 0 ? &prefix##_ops[1] : &prefix##_ops[2])

struct __lockable _FILE {
  // Constant data.
  int fd;                        // Underlying file descriptor.
  const struct fileops *ops;     // Underlying operations.
  const struct lc_ctype *ctype;  // Character set or wide character conversion.

  // Variable data.
  enum {
    F_EOF = 0x1,    // Stream is at end-of-file.
    F_ERROR = 0x2,  // An I/O error has occurred.
  } flags;
  int orientation;    // Wide-oriented or byte-oriented stream.
  int buftype;        // _IOFBF, _IOLBF or _IONBF.
  bool should_flush;  // Whether funlockfile() should flush.
  off_t offset;       // Negative if stream is not seekable.

  char ungetc[MB_LEN_MAX * 4];  // ungetc() buffer. Filled from the back.
  size_t ungetclen;             // Number of characters pushed back.

  char *readbuf;
  size_t readbuflen;
  mbstate_t readstate;

  char *writebuf;
  size_t writebuflen;

  // Data associated with this object.
  union {
    struct {
      char *buf;
      const char *owritebuf;
      size_t used;
      size_t size;
      bool external : 1;
      bool append : 1;
    } fmemopen;
    struct {
      char *buf;
      char *written;
      size_t bufsize;
      bool append : 1;
    } file;
    struct {
      char *buf;
      const char *owritebuf;
      size_t used;
      size_t size;
      char **bufp;
      size_t *sizep;
    } memstream;
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

  // Per-stream lock. These must remain the last member of the object,
  // as fswap() will copy all structure members up to the lock.
  pthread_mutex_t lock;
  unsigned int lockcount;
#define FILE_COPYSIZE offsetof(FILE, lock)
};

static inline bool fseekable(FILE *stream) __requires_exclusive(*stream) {
  return stream->offset >= 0;
}

// Invocation of operations.

static inline bool fop_read_peek(FILE *stream) __requires_exclusive(*stream) {
  assert(stream->readbuflen == 0 && "Reading while data is already present");
  assert(stream->ungetclen == 0 && "Reading while ungetc chars are present");
  if (stream->ops->read_peek(stream)) {
    assert((!fseekable(stream) || stream->writebuflen == 0) &&
           "Write buffer still left intact");
    if (stream->readbuflen == 0)
      stream->flags |= F_EOF;
    return true;
  }
  stream->flags |= F_ERROR;
  return false;
}

static inline bool fop_write_peek(FILE *stream) __requires_exclusive(*stream) {
  if (stream->ops->write_peek(stream)) {
    assert(stream->writebuflen > 0 &&
           "Write flushing did not yield a new write buffer");
    assert((!fseekable(stream) || stream->readbuflen == 0) &&
           "Read buffer still left intact");
    return true;
  }
  stream->flags |= F_ERROR;
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
  if (stream->ops->flush(stream))
    return true;
  stream->flags |= F_ERROR;
  return false;
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
  ++file->lockcount;
}

static inline int __ftrylockfile(FILE *file)
    __trylocks_exclusive(0, *file) __no_lock_analysis {
  if (pthread_mutex_trylock(&file->lock) != 0)
    return 1;
  ++file->lockcount;
  return 0;
}

static inline void __funlockfile(FILE *file)
    __unlocks(*file) __no_lock_analysis {
  assert(file->lockcount > 0 && "Invalid lock recursion count");
  if (--file->lockcount == 0 && file->should_flush) {
    fop_flush(file);
    file->should_flush = false;
  }
  pthread_mutex_unlock(&file->lock);
}

#define flockfile(file) __flockfile(file)
#define ftrylockfile(file) __ftrylockfile(file)
#define funlockfile(file) __funlockfile(file)

static inline void flockfile_orientation(FILE *file, int mode)
    __locks_exclusive(*file) {
  flockfile(file);
  if (file->orientation == 0)
    file->orientation = mode;
}

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
  if (stream->readbuflen == 0 && !fop_read_peek(stream))
    return false;

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

static inline bool fwrite_peek(FILE *stream, char **writebuf,
                               size_t *writebuflen)
    __requires_exclusive(*stream) {
  if (stream->writebuflen == 0 && !fop_write_peek(stream))
    return false;
  *writebuf = stream->writebuf;
  *writebuflen = stream->writebuflen;
  return true;
}

static inline void fwrite_produce(FILE *stream, size_t buflen)
    __requires_exclusive(*stream) {
  // Determine whether funlockfile() should flush.
  if ((stream->buftype == _IOLBF &&
       memchr(stream->writebuf, '\n', buflen) != NULL) ||
      (stream->buftype == _IONBF && buflen > 0))
    stream->should_flush = true;

  // Progress write buffer pointer.
  assert(buflen <= stream->writebuflen &&
         "Attempted to produce more data than the buffer can hold");
  stream->writebuf += buflen;
  stream->writebuflen -= buflen;
}

static inline size_t fwrite_put(FILE *stream, const char *buf, size_t inbuflen)
    __requires_exclusive(*stream) {
  assert(inbuflen > 0 && "Attempted to write zero bytes");

  const char *inbuf = buf;
  for (;;) {
    char *writebuf;
    size_t writebuflen;
    if (!fwrite_peek(stream, &writebuf, &writebuflen))
      return inbuf - buf;
    if (inbuflen > writebuflen) {
      memcpy(writebuf, inbuf, writebuflen);
      fwrite_produce(stream, writebuflen);
      inbuf += writebuflen;
      inbuflen -= writebuflen;
    } else {
      memcpy(writebuf, inbuf, inbuflen);
      fwrite_produce(stream, inbuflen);
      return inbuf + inbuflen - buf;
    }
  }
}

static inline int __putc_unlocked(int c, FILE *stream)
    __requires_exclusive(*stream) {
  char *writebuf;
  size_t writebuflen;
  if (!fwrite_peek(stream, &writebuf, &writebuflen))
    return EOF;
  *writebuf = c;
  fwrite_produce(stream, 1);
  return (unsigned char)c;
}
#define putc_unlocked(c, stream) __putc_unlocked(c, stream)

static inline wint_t putwc_unlocked(wchar_t wc, FILE *stream)
    __requires_exclusive(*stream) {
  // Convert character to a multibyte sequence, using the character set
  // associated with the stream.
  const struct lc_ctype *ctype = stream->ctype;
  char buf[MB_LEN_MAX];
  ssize_t len = ctype->c32tomb(buf, wc, ctype->data);
  if (len == -1) {
    stream->flags |= F_ERROR;
    return WEOF;
  }

  // Write it.
  size_t written = fwrite_put(stream, buf, len);
  return written == (size_t)len ? wc : WEOF;
}

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
FILE *__falloc(locale_t locale);

#endif
