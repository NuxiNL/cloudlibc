// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <uv.h>

static_assert(UV_E2BIG == -E2BIG, "Value mismatch");
static_assert(UV_EACCES == -EACCES, "Value mismatch");
static_assert(UV_EADDRINUSE == -EADDRINUSE, "Value mismatch");
static_assert(UV_EADDRNOTAVAIL == -EADDRNOTAVAIL, "Value mismatch");
static_assert(UV_EAFNOSUPPORT == -EAFNOSUPPORT, "Value mismatch");
static_assert(UV_EAGAIN == -EAGAIN, "Value mismatch");
static_assert(UV_EALREADY == -EALREADY, "Value mismatch");
static_assert(UV_EBADF == -EBADF, "Value mismatch");
static_assert(UV_EBUSY == -EBUSY, "Value mismatch");
static_assert(UV_ECANCELED == -ECANCELED, "Value mismatch");
static_assert(UV_ECONNABORTED == -ECONNABORTED, "Value mismatch");
static_assert(UV_ECONNREFUSED == -ECONNREFUSED, "Value mismatch");
static_assert(UV_ECONNRESET == -ECONNRESET, "Value mismatch");
static_assert(UV_EDESTADDRREQ == -EDESTADDRREQ, "Value mismatch");
static_assert(UV_EEXIST == -EEXIST, "Value mismatch");
static_assert(UV_EFAULT == -EFAULT, "Value mismatch");
static_assert(UV_EFBIG == -EFBIG, "Value mismatch");
static_assert(UV_EHOSTUNREACH == -EHOSTUNREACH, "Value mismatch");
static_assert(UV_EINTR == -EINTR, "Value mismatch");
static_assert(UV_EINVAL == -EINVAL, "Value mismatch");
static_assert(UV_EIO == -EIO, "Value mismatch");
static_assert(UV_EISCONN == -EISCONN, "Value mismatch");
static_assert(UV_EISDIR == -EISDIR, "Value mismatch");
static_assert(UV_ELOOP == -ELOOP, "Value mismatch");
static_assert(UV_EMFILE == -EMFILE, "Value mismatch");
static_assert(UV_EMLINK == -EMLINK, "Value mismatch");
static_assert(UV_EMSGSIZE == -EMSGSIZE, "Value mismatch");
static_assert(UV_ENAMETOOLONG == -ENAMETOOLONG, "Value mismatch");
static_assert(UV_ENETDOWN == -ENETDOWN, "Value mismatch");
static_assert(UV_ENETUNREACH == -ENETUNREACH, "Value mismatch");
static_assert(UV_ENFILE == -ENFILE, "Value mismatch");
static_assert(UV_ENOBUFS == -ENOBUFS, "Value mismatch");
static_assert(UV_ENODEV == -ENODEV, "Value mismatch");
static_assert(UV_ENOENT == -ENOENT, "Value mismatch");
static_assert(UV_ENOMEM == -ENOMEM, "Value mismatch");
static_assert(UV_ENOPROTOOPT == -ENOPROTOOPT, "Value mismatch");
static_assert(UV_ENOSPC == -ENOSPC, "Value mismatch");
static_assert(UV_ENOSYS == -ENOSYS, "Value mismatch");
static_assert(UV_ENOTCONN == -ENOTCONN, "Value mismatch");
static_assert(UV_ENOTDIR == -ENOTDIR, "Value mismatch");
static_assert(UV_ENOTEMPTY == -ENOTEMPTY, "Value mismatch");
static_assert(UV_ENOTSOCK == -ENOTSOCK, "Value mismatch");
static_assert(UV_ENOTSUP == -ENOTSUP, "Value mismatch");
static_assert(UV_ENOTTY == -ENOTTY, "Value mismatch");
static_assert(UV_ENXIO == -ENXIO, "Value mismatch");
static_assert(UV_EPERM == -EPERM, "Value mismatch");
static_assert(UV_EPIPE == -EPIPE, "Value mismatch");
static_assert(UV_EPROTO == -EPROTO, "Value mismatch");
static_assert(UV_EPROTONOSUPPORT == -EPROTONOSUPPORT, "Value mismatch");
static_assert(UV_EPROTOTYPE == -EPROTOTYPE, "Value mismatch");
static_assert(UV_ERANGE == -ERANGE, "Value mismatch");
static_assert(UV_EROFS == -EROFS, "Value mismatch");
static_assert(UV_ESPIPE == -ESPIPE, "Value mismatch");
static_assert(UV_ESRCH == -ESRCH, "Value mismatch");
static_assert(UV_ETIMEDOUT == -ETIMEDOUT, "Value mismatch");
static_assert(UV_ETXTBSY == -ETXTBSY, "Value mismatch");
static_assert(UV_EXDEV == -EXDEV, "Value mismatch");

static_assert(UV_EAI_AGAIN == -1000 - EAI_AGAIN, "Value mismatch");
static_assert(UV_EAI_BADFLAGS == -1000 - EAI_BADFLAGS, "Value mismatch");
static_assert(UV_EAI_FAIL == -1000 - EAI_FAIL, "Value mismatch");
static_assert(UV_EAI_FAMILY == -1000 - EAI_FAMILY, "Value mismatch");
static_assert(UV_EAI_MEMORY == -1000 - EAI_MEMORY, "Value mismatch");
static_assert(UV_EAI_NONAME == -1000 - EAI_NONAME, "Value mismatch");
static_assert(UV_EAI_OVERFLOW == -1000 - EAI_OVERFLOW, "Value mismatch");
static_assert(UV_EAI_SERVICE == -1000 - EAI_SERVICE, "Value mismatch");
static_assert(UV_EAI_SOCKTYPE == -1000 - EAI_SOCKTYPE, "Value mismatch");

int uv_translate_sys_error(int sys_errno) {
  return sys_errno < 0 ? sys_errno : -sys_errno;
}
