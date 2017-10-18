// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

const char *uv_strerror(int err) {
  switch (err) {
    case UV_E2BIG:
      return "argument list too long";
    case UV_EACCES:
      return "permission denied";
    case UV_EADDRINUSE:
      return "address already in use";
    case UV_EADDRNOTAVAIL:
      return "address not available";
    case UV_EAFNOSUPPORT:
      return "address family not supported";
    case UV_EAGAIN:
      return "resource temporarily unavailable";
    case UV_EAI_ADDRFAMILY:
      return "address family not supported";
    case UV_EAI_AGAIN:
      return "temporary failure";
    case UV_EAI_BADFLAGS:
      return "bad ai_flags value";
    case UV_EAI_BADHINTS:
      return "invalid value for hints";
    case UV_EAI_CANCELED:
      return "request canceled";
    case UV_EAI_FAIL:
      return "permanent failure";
    case UV_EAI_FAMILY:
      return "ai_family not supported";
    case UV_EAI_MEMORY:
      return "out of memory";
    case UV_EAI_NODATA:
      return "no address";
    case UV_EAI_NONAME:
      return "unknown node or service";
    case UV_EAI_OVERFLOW:
      return "argument buffer overflow";
    case UV_EAI_PROTOCOL:
      return "resolved protocol is unknown";
    case UV_EAI_SERVICE:
      return "service not available for socket type";
    case UV_EAI_SOCKTYPE:
      return "socket type not supported";
    case UV_EALREADY:
      return "connection already in progress";
    case UV_EBADF:
      return "bad file descriptor";
    case UV_EBUSY:
      return "resource busy or locked";
    case UV_ECANCELED:
      return "operation canceled";
    case UV_ECHARSET:
      return "invalid Unicode character";
    case UV_ECONNABORTED:
      return "software caused connection abort";
    case UV_ECONNREFUSED:
      return "connection refused";
    case UV_ECONNRESET:
      return "connection reset by peer";
    case UV_EDESTADDRREQ:
      return "destination address required";
    case UV_EEXIST:
      return "file already exists";
    case UV_EFAULT:
      return "bad address in system call argument";
    case UV_EFBIG:
      return "file too large";
    case UV_EHOSTUNREACH:
      return "host is unreachable";
    case UV_EINTR:
      return "interrupted system call";
    case UV_EINVAL:
      return "invalid argument";
    case UV_EIO:
      return "i/o error";
    case UV_EISCONN:
      return "socket is already connected";
    case UV_EISDIR:
      return "illegal operation on a directory";
    case UV_ELOOP:
      return "too many symbolic links encountered";
    case UV_EMFILE:
      return "too many open files";
    case UV_EMLINK:
      return "too many links";
    case UV_EMSGSIZE:
      return "message too long";
    case UV_ENAMETOOLONG:
      return "name too long";
    case UV_ENETDOWN:
      return "network is down";
    case UV_ENETUNREACH:
      return "network is unreachable";
    case UV_ENFILE:
      return "file table overflow";
    case UV_ENOBUFS:
      return "no buffer space available";
    case UV_ENODEV:
      return "no such device";
    case UV_ENOENT:
      return "no such file or directory";
    case UV_ENOMEM:
      return "not enough memory";
    case UV_ENONET:
      return "machine is not on the network";
    case UV_ENOPROTOOPT:
      return "protocol not available";
    case UV_ENOSPC:
      return "no space left on device";
    case UV_ENOSYS:
      return "function not implemented";
    case UV_ENOTCONN:
      return "socket is not connected";
    case UV_ENOTDIR:
      return "not a directory";
    case UV_ENOTEMPTY:
      return "directory not empty";
    case UV_ENOTSOCK:
      return "socket operation on non-socket";
    case UV_ENOTSUP:
      return "operation not supported on socket";
    case UV_ENXIO:
      return "no such device or address";
    case UV_EOF:
      return "end of file";
    case UV_EPERM:
      return "operation not permitted";
    case UV_EPIPE:
      return "broken pipe";
    case UV_EPROTO:
      return "protocol error";
    case UV_EPROTONOSUPPORT:
      return "protocol not supported";
    case UV_EPROTOTYPE:
      return "protocol wrong type for socket";
    case UV_ERANGE:
      return "result too large";
    case UV_EROFS:
      return "read-only file system";
    case UV_ESHUTDOWN:
      return "cannot send after transport endpoint shutdown";
    case UV_ESPIPE:
      return "invalid seek";
    case UV_ESRCH:
      return "no such process";
    case UV_ETIMEDOUT:
      return "connection timed out";
    case UV_ETXTBSY:
      return "text file is busy";
    case UV_EXDEV:
      return "cross-device link not permitted";
    case UV_UNKNOWN:
      return "unknown error";
    default:
      return __uv_strerror_unknown(err);
  }
}
