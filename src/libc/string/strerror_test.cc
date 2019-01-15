// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(strerror, examples) {
#define TEST_ERRNO(code, str)          \
  do {                                 \
    errno = 1234;                      \
    ASSERT_STREQ(str, strerror(code)); \
    ASSERT_EQ(1234, errno);            \
  } while (0)
  TEST_ERRNO(-1, "Unknown error");
  TEST_ERRNO(0, "Success");

  TEST_ERRNO(E2BIG, "Argument list too long");
  TEST_ERRNO(EACCES, "Permission denied");
  TEST_ERRNO(EADDRINUSE, "Address already in use");
  TEST_ERRNO(EADDRNOTAVAIL, "Can't assign requested address");
  TEST_ERRNO(EAFNOSUPPORT, "Address family not supported by protocol family");
  TEST_ERRNO(EAGAIN, "Resource temporarily unavailable");
  TEST_ERRNO(EALREADY, "Operation already in progress");
  TEST_ERRNO(EBADF, "Bad file descriptor");
  TEST_ERRNO(EBADMSG, "Bad message");
  TEST_ERRNO(EBUSY, "Device busy");
  TEST_ERRNO(ECANCELED, "Operation canceled");
  TEST_ERRNO(ECHILD, "No child processes");
  TEST_ERRNO(ECONNABORTED, "Software caused connection abort");
  TEST_ERRNO(ECONNREFUSED, "Connection refused");
  TEST_ERRNO(ECONNRESET, "Connection reset by peer");
  TEST_ERRNO(EDEADLK, "Resource deadlock avoided");
  TEST_ERRNO(EDESTADDRREQ, "Destination address required");
  TEST_ERRNO(EDOM, "Numerical argument out of domain");
  TEST_ERRNO(EDQUOT, "Disc quota exceeded");
  TEST_ERRNO(EEXIST, "File exists");
  TEST_ERRNO(EFAULT, "Bad address");
  TEST_ERRNO(EFBIG, "File too large");
  TEST_ERRNO(EHOSTUNREACH, "No route to host");
  TEST_ERRNO(EIDRM, "Identifier removed");
  TEST_ERRNO(EILSEQ, "Illegal byte sequence");
  TEST_ERRNO(EINPROGRESS, "Operation now in progress");
  TEST_ERRNO(EINTR, "Interrupted system call");
  TEST_ERRNO(EINVAL, "Invalid argument");
  TEST_ERRNO(EIO, "Input/output error");
  TEST_ERRNO(EISCONN, "Socket is already connected");
  TEST_ERRNO(EISDIR, "Is a directory");
  TEST_ERRNO(ELOOP, "Too many levels of symbolic links");
  TEST_ERRNO(EMFILE, "Too many open files");
  TEST_ERRNO(EMLINK, "Too many links");
  TEST_ERRNO(EMSGSIZE, "Message too long");
  TEST_ERRNO(EMULTIHOP, "Multihop attempted");
  TEST_ERRNO(ENAMETOOLONG, "File name too long");
  TEST_ERRNO(ENETDOWN, "Network is down");
  TEST_ERRNO(ENETRESET, "Network dropped connection on reset");
  TEST_ERRNO(ENETUNREACH, "Network is unreachable");
  TEST_ERRNO(ENFILE, "Too many open files in system");
  TEST_ERRNO(ENOBUFS, "No buffer space available");
  TEST_ERRNO(ENODEV, "Operation not supported by device");
  TEST_ERRNO(ENOENT, "No such file or directory");
  TEST_ERRNO(ENOEXEC, "Exec format error");
  TEST_ERRNO(ENOLCK, "No locks available");
  TEST_ERRNO(ENOLINK, "Link has been severed");
  TEST_ERRNO(ENOMEM, "Cannot allocate memory");
  TEST_ERRNO(ENOMSG, "No message of desired type");
  TEST_ERRNO(ENOPROTOOPT, "Protocol not available");
  TEST_ERRNO(ENOSPC, "No space left on device");
  TEST_ERRNO(ENOSYS, "Function not implemented");
  TEST_ERRNO(ENOTCAPABLE, "Capabilities insufficient");
  TEST_ERRNO(ENOTCONN, "Socket is not connected");
  TEST_ERRNO(ENOTDIR, "Not a directory");
  TEST_ERRNO(ENOTEMPTY, "Directory not empty");
  TEST_ERRNO(ENOTRECOVERABLE, "State not recoverable");
  TEST_ERRNO(ENOTSOCK, "Socket operation on non-socket");
  TEST_ERRNO(ENOTSUP, "Operation not supported");
  TEST_ERRNO(ENOTTY, "Inappropriate ioctl for device");
  TEST_ERRNO(ENXIO, "Device not configured");
  TEST_ERRNO(EOPNOTSUPP, "Operation not supported");
  TEST_ERRNO(EOVERFLOW, "Value too large to be stored in data type");
  TEST_ERRNO(EOWNERDEAD, "Previous owner died");
  TEST_ERRNO(EPERM, "Operation not permitted");
  TEST_ERRNO(EPIPE, "Broken pipe");
  TEST_ERRNO(EPROTO, "Protocol error");
  TEST_ERRNO(EPROTONOSUPPORT, "Protocol not supported");
  TEST_ERRNO(EPROTOTYPE, "Protocol wrong type for socket");
  TEST_ERRNO(ERANGE, "Result too large");
  TEST_ERRNO(EROFS, "Read-only file system");
  TEST_ERRNO(ESPIPE, "Illegal seek");
  TEST_ERRNO(ESRCH, "No such process");
  TEST_ERRNO(ESTALE, "Stale NFS file handle");
  TEST_ERRNO(ETIMEDOUT, "Operation timed out");
  TEST_ERRNO(ETXTBSY, "Text file busy");
  TEST_ERRNO(EWOULDBLOCK, "Resource temporarily unavailable");
  TEST_ERRNO(EXDEV, "Cross-device link");
#undef TEST_ERRNO
}
