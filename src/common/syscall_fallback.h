// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_SYSCALL_FALLBACK_H
#define COMMON_SYSCALL_FALLBACK_H

#include <cloudabi_syscalls_struct.h>
#include <cloudabi_types.h>
#include <stdint.h>
#include <stdnoreturn.h>

// Fallbacks for invoking system calls.
//
// We're currently in a transition where executables should no longer
// try to use magic instructions like "syscall" to invoke system calls.
// Instead, they should only use the functions provided by the vDSO.
//
// As there are still environments that don't provide a vDSO yet,
// provide fallback functions. We shouldn't add implementations for new
// system calls or architectures to this file. Those should use the vDSO
// exclusively.

#if defined(__aarch64__)

static cloudabi_errno_t syscall_fallback_clock_res_get(
    cloudabi_clockid_t clock_id, cloudabi_timestamp_t *resolution) {
  register uint64_t reg_x8 asm("x8") = 0;
  register uint64_t reg_x0 asm("x0") = (uint64_t)clock_id;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *resolution = (cloudabi_timestamp_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_clock_time_get(
    cloudabi_clockid_t clock_id, cloudabi_timestamp_t precision,
    cloudabi_timestamp_t *time) {
  register uint64_t reg_x8 asm("x8") = 1;
  register uint64_t reg_x0 asm("x0") = (uint64_t)clock_id;
  register uint64_t reg_x1 asm("x1") = (uint64_t)precision;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *time = (cloudabi_timestamp_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_condvar_signal(
    _Atomic(cloudabi_condvar_t) * condvar, cloudabi_scope_t scope,
    cloudabi_nthreads_t nwaiters) {
  register uint64_t reg_x8 asm("x8") = 2;
  register uint64_t reg_x0 asm("x0") = (uint64_t)condvar;
  register uint64_t reg_x1 asm("x1") = (uint64_t)scope;
  register uint64_t reg_x2 asm("x2") = (uint64_t)nwaiters;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_close(cloudabi_fd_t fd) {
  register uint64_t reg_x8 asm("x8") = 3;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_create1(cloudabi_filetype_t type,
                                                    cloudabi_fd_t *fd) {
  register uint64_t reg_x8 asm("x8") = 4;
  register uint64_t reg_x0 asm("x0") = (uint64_t)type;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_create2(cloudabi_filetype_t type,
                                                    cloudabi_fd_t *fd1,
                                                    cloudabi_fd_t *fd2) {
  register uint64_t reg_x8 asm("x8") = 5;
  register uint64_t reg_x0 asm("x0") = (uint64_t)type;
  register uint64_t reg_x1 asm("x1");
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0), "=r"(reg_x1)
      : "r"(reg_x8), "r"(reg_x0)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *fd1 = (cloudabi_fd_t)reg_x0;
    *fd2 = (cloudabi_fd_t)reg_x1;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_datasync(cloudabi_fd_t fd) {
  register uint64_t reg_x8 asm("x8") = 6;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_dup(cloudabi_fd_t from,
                                                cloudabi_fd_t *fd) {
  register uint64_t reg_x8 asm("x8") = 7;
  register uint64_t reg_x0 asm("x0") = (uint64_t)from;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_pread(cloudabi_fd_t fd,
                                                  const cloudabi_iovec_t *iov,
                                                  size_t iovcnt,
                                                  cloudabi_filesize_t offset,
                                                  size_t *nread) {
  register uint64_t reg_x8 asm("x8") = 8;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)iov;
  register uint64_t reg_x2 asm("x2") = (uint64_t)iovcnt;
  register uint64_t reg_x3 asm("x3") = (uint64_t)offset;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *nread = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_pwrite(cloudabi_fd_t fd,
                                                   const cloudabi_ciovec_t *iov,
                                                   size_t iovcnt,
                                                   cloudabi_filesize_t offset,
                                                   size_t *nwritten) {
  register uint64_t reg_x8 asm("x8") = 9;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)iov;
  register uint64_t reg_x2 asm("x2") = (uint64_t)iovcnt;
  register uint64_t reg_x3 asm("x3") = (uint64_t)offset;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *nwritten = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_read(cloudabi_fd_t fd,
                                                 const cloudabi_iovec_t *iov,
                                                 size_t iovcnt, size_t *nread) {
  register uint64_t reg_x8 asm("x8") = 10;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)iov;
  register uint64_t reg_x2 asm("x2") = (uint64_t)iovcnt;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *nread = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_replace(cloudabi_fd_t from,
                                                    cloudabi_fd_t to) {
  register uint64_t reg_x8 asm("x8") = 11;
  register uint64_t reg_x0 asm("x0") = (uint64_t)from;
  register uint64_t reg_x1 asm("x1") = (uint64_t)to;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_seek(
    cloudabi_fd_t fd, cloudabi_filedelta_t offset, cloudabi_whence_t whence,
    cloudabi_filesize_t *newoffset) {
  register uint64_t reg_x8 asm("x8") = 12;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)offset;
  register uint64_t reg_x2 asm("x2") = (uint64_t)whence;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *newoffset = (cloudabi_filesize_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_stat_get(cloudabi_fd_t fd,
                                                     cloudabi_fdstat_t *buf) {
  register uint64_t reg_x8 asm("x8") = 13;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)buf;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_stat_put(
    cloudabi_fd_t fd, const cloudabi_fdstat_t *buf, cloudabi_fdsflags_t flags) {
  register uint64_t reg_x8 asm("x8") = 14;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)buf;
  register uint64_t reg_x2 asm("x2") = (uint64_t)flags;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_sync(cloudabi_fd_t fd) {
  register uint64_t reg_x8 asm("x8") = 15;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_fd_write(cloudabi_fd_t fd,
                                                  const cloudabi_ciovec_t *iov,
                                                  size_t iovcnt,
                                                  size_t *nwritten) {
  register uint64_t reg_x8 asm("x8") = 16;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)iov;
  register uint64_t reg_x2 asm("x2") = (uint64_t)iovcnt;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *nwritten = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_advise(cloudabi_fd_t fd,
                                                     cloudabi_filesize_t offset,
                                                     cloudabi_filesize_t len,
                                                     cloudabi_advice_t advice) {
  register uint64_t reg_x8 asm("x8") = 17;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)offset;
  register uint64_t reg_x2 asm("x2") = (uint64_t)len;
  register uint64_t reg_x3 asm("x3") = (uint64_t)advice;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_allocate(
    cloudabi_fd_t fd, cloudabi_filesize_t offset, cloudabi_filesize_t len) {
  register uint64_t reg_x8 asm("x8") = 18;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)offset;
  register uint64_t reg_x2 asm("x2") = (uint64_t)len;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_create(cloudabi_fd_t fd,
                                                     const char *path,
                                                     size_t pathlen,
                                                     cloudabi_filetype_t type) {
  register uint64_t reg_x8 asm("x8") = 19;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path;
  register uint64_t reg_x2 asm("x2") = (uint64_t)pathlen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)type;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_link(
    cloudabi_lookup_t fd1, const char *path1, size_t path1len,
    cloudabi_fd_t fd2, const char *path2, size_t path2len) {
  register uint64_t reg_x8 asm("x8") = 20;
  register uint64_t reg_x0 asm("x0") = *(uint64_t *)&fd1;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path1;
  register uint64_t reg_x2 asm("x2") = (uint64_t)path1len;
  register uint64_t reg_x3 asm("x3") = (uint64_t)fd2;
  register uint64_t reg_x4 asm("x4") = (uint64_t)path2;
  register uint64_t reg_x5 asm("x5") = (uint64_t)path2len;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2),
                 "r"(reg_x3), "r"(reg_x4), "r"(reg_x5)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_open(
    cloudabi_lookup_t dirfd, const char *path, size_t pathlen,
    cloudabi_oflags_t oflags, const cloudabi_fdstat_t *fds, cloudabi_fd_t *fd) {
  register uint64_t reg_x8 asm("x8") = 21;
  register uint64_t reg_x0 asm("x0") = *(uint64_t *)&dirfd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path;
  register uint64_t reg_x2 asm("x2") = (uint64_t)pathlen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)oflags;
  register uint64_t reg_x4 asm("x4") = (uint64_t)fds;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3),
        "r"(reg_x4)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_readdir(
    cloudabi_fd_t fd, void *buf, size_t nbyte, cloudabi_dircookie_t cookie,
    size_t *bufused) {
  register uint64_t reg_x8 asm("x8") = 22;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)buf;
  register uint64_t reg_x2 asm("x2") = (uint64_t)nbyte;
  register uint64_t reg_x3 asm("x3") = (uint64_t)cookie;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *bufused = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_readlink(
    cloudabi_fd_t fd, const char *path, size_t pathlen, char *buf,
    size_t bufsize, size_t *bufused) {
  register uint64_t reg_x8 asm("x8") = 23;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path;
  register uint64_t reg_x2 asm("x2") = (uint64_t)pathlen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)buf;
  register uint64_t reg_x4 asm("x4") = (uint64_t)bufsize;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3),
        "r"(reg_x4)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *bufused = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_rename(
    cloudabi_fd_t oldfd, const char *old, size_t oldlen, cloudabi_fd_t newfd,
    const char *new, size_t newlen) {
  register uint64_t reg_x8 asm("x8") = 24;
  register uint64_t reg_x0 asm("x0") = (uint64_t)oldfd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)old;
  register uint64_t reg_x2 asm("x2") = (uint64_t)oldlen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)newfd;
  register uint64_t reg_x4 asm("x4") = (uint64_t) new;
  register uint64_t reg_x5 asm("x5") = (uint64_t)newlen;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2),
                 "r"(reg_x3), "r"(reg_x4), "r"(reg_x5)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_stat_fget(
    cloudabi_fd_t fd, cloudabi_filestat_t *buf) {
  register uint64_t reg_x8 asm("x8") = 25;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)buf;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_stat_fput(
    cloudabi_fd_t fd, const cloudabi_filestat_t *buf,
    cloudabi_fsflags_t flags) {
  register uint64_t reg_x8 asm("x8") = 26;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)buf;
  register uint64_t reg_x2 asm("x2") = (uint64_t)flags;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_stat_get(
    cloudabi_lookup_t fd, const char *path, size_t pathlen,
    cloudabi_filestat_t *buf) {
  register uint64_t reg_x8 asm("x8") = 27;
  register uint64_t reg_x0 asm("x0") = *(uint64_t *)&fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path;
  register uint64_t reg_x2 asm("x2") = (uint64_t)pathlen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)buf;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_stat_put(
    cloudabi_lookup_t fd, const char *path, size_t pathlen,
    const cloudabi_filestat_t *buf, cloudabi_fsflags_t flags) {
  register uint64_t reg_x8 asm("x8") = 28;
  register uint64_t reg_x0 asm("x0") = *(uint64_t *)&fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path;
  register uint64_t reg_x2 asm("x2") = (uint64_t)pathlen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)buf;
  register uint64_t reg_x4 asm("x4") = (uint64_t)flags;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2),
                 "r"(reg_x3), "r"(reg_x4)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_symlink(const char *path1,
                                                      size_t path1len,
                                                      cloudabi_fd_t fd,
                                                      const char *path2,
                                                      size_t path2len) {
  register uint64_t reg_x8 asm("x8") = 29;
  register uint64_t reg_x0 asm("x0") = (uint64_t)path1;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path1len;
  register uint64_t reg_x2 asm("x2") = (uint64_t)fd;
  register uint64_t reg_x3 asm("x3") = (uint64_t)path2;
  register uint64_t reg_x4 asm("x4") = (uint64_t)path2len;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2),
                 "r"(reg_x3), "r"(reg_x4)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_file_unlink(cloudabi_fd_t fd,
                                                     const char *path,
                                                     size_t pathlen,
                                                     cloudabi_ulflags_t flags) {
  register uint64_t reg_x8 asm("x8") = 30;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)path;
  register uint64_t reg_x2 asm("x2") = (uint64_t)pathlen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)flags;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_lock_unlock(_Atomic(cloudabi_lock_t) *
                                                         lock,
                                                     cloudabi_scope_t scope) {
  register uint64_t reg_x8 asm("x8") = 31;
  register uint64_t reg_x0 asm("x0") = (uint64_t)lock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)scope;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_mem_advise(void *addr, size_t len,
                                                    cloudabi_advice_t advice) {
  register uint64_t reg_x8 asm("x8") = 32;
  register uint64_t reg_x0 asm("x0") = (uint64_t)addr;
  register uint64_t reg_x1 asm("x1") = (uint64_t)len;
  register uint64_t reg_x2 asm("x2") = (uint64_t)advice;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_mem_lock(const void *addr,
                                                  size_t len) {
  register uint64_t reg_x8 asm("x8") = 33;
  register uint64_t reg_x0 asm("x0") = (uint64_t)addr;
  register uint64_t reg_x1 asm("x1") = (uint64_t)len;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_mem_map(
    void *addr, size_t len, cloudabi_mprot_t prot, cloudabi_mflags_t flags,
    cloudabi_fd_t fd, cloudabi_filesize_t off, void **mem) {
  register uint64_t reg_x8 asm("x8") = 34;
  register uint64_t reg_x0 asm("x0") = (uint64_t)addr;
  register uint64_t reg_x1 asm("x1") = (uint64_t)len;
  register uint64_t reg_x2 asm("x2") = (uint64_t)prot;
  register uint64_t reg_x3 asm("x3") = (uint64_t)flags;
  register uint64_t reg_x4 asm("x4") = (uint64_t)fd;
  register uint64_t reg_x5 asm("x5") = (uint64_t)off;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3),
        "r"(reg_x4), "r"(reg_x5)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *mem = (void *)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_mem_protect(void *addr, size_t len,
                                                     cloudabi_mprot_t prot) {
  register uint64_t reg_x8 asm("x8") = 35;
  register uint64_t reg_x0 asm("x0") = (uint64_t)addr;
  register uint64_t reg_x1 asm("x1") = (uint64_t)len;
  register uint64_t reg_x2 asm("x2") = (uint64_t)prot;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_mem_sync(void *addr, size_t len,
                                                  cloudabi_msflags_t flags) {
  register uint64_t reg_x8 asm("x8") = 36;
  register uint64_t reg_x0 asm("x0") = (uint64_t)addr;
  register uint64_t reg_x1 asm("x1") = (uint64_t)len;
  register uint64_t reg_x2 asm("x2") = (uint64_t)flags;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_mem_unlock(const void *addr,
                                                    size_t len) {
  register uint64_t reg_x8 asm("x8") = 37;
  register uint64_t reg_x0 asm("x0") = (uint64_t)addr;
  register uint64_t reg_x1 asm("x1") = (uint64_t)len;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_mem_unmap(void *addr, size_t len) {
  register uint64_t reg_x8 asm("x8") = 38;
  register uint64_t reg_x0 asm("x0") = (uint64_t)addr;
  register uint64_t reg_x1 asm("x1") = (uint64_t)len;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_poll(const cloudabi_subscription_t *in,
                                              cloudabi_event_t *out,
                                              size_t nsubscriptions,
                                              size_t *nevents) {
  register uint64_t reg_x8 asm("x8") = 39;
  register uint64_t reg_x0 asm("x0") = (uint64_t)in;
  register uint64_t reg_x1 asm("x1") = (uint64_t)out;
  register uint64_t reg_x2 asm("x2") = (uint64_t)nsubscriptions;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *nevents = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_poll_fd(
    cloudabi_fd_t fd, const cloudabi_subscription_t *in, size_t nin,
    cloudabi_event_t *out, size_t nout, const cloudabi_subscription_t *timeout,
    size_t *nevents) {
  register uint64_t reg_x8 asm("x8") = 57;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)in;
  register uint64_t reg_x2 asm("x2") = (uint64_t)nin;
  register uint64_t reg_x3 asm("x3") = (uint64_t)out;
  register uint64_t reg_x4 asm("x4") = (uint64_t)nout;
  register uint64_t reg_x5 asm("x5") = (uint64_t)timeout;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3),
        "r"(reg_x4), "r"(reg_x5)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *nevents = (size_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_proc_exec(cloudabi_fd_t fd,
                                                   const void *data,
                                                   size_t datalen,
                                                   const cloudabi_fd_t *fds,
                                                   size_t fdslen) {
  register uint64_t reg_x8 asm("x8") = 40;
  register uint64_t reg_x0 asm("x0") = (uint64_t)fd;
  register uint64_t reg_x1 asm("x1") = (uint64_t)data;
  register uint64_t reg_x2 asm("x2") = (uint64_t)datalen;
  register uint64_t reg_x3 asm("x3") = (uint64_t)fds;
  register uint64_t reg_x4 asm("x4") = (uint64_t)fdslen;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2),
                 "r"(reg_x3), "r"(reg_x4)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static noreturn void syscall_fallback_proc_exit(cloudabi_exitcode_t rval) {
  register uint64_t reg_x8 asm("x8") = 41;
  register uint64_t reg_x0 asm("x0") = (uint64_t)rval;
  asm volatile("\tsvc 0\n"
               :
               : "r"(reg_x8), "r"(reg_x0)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  for (;;)
    ;
}

static cloudabi_errno_t syscall_fallback_proc_fork(cloudabi_fd_t *fd,
                                                   cloudabi_tid_t *tid) {
  register uint64_t reg_x8 asm("x8") = 42;
  register uint64_t reg_x0 asm("x0");
  register uint64_t reg_x1 asm("x1");
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0), "=r"(reg_x1)
      : "r"(reg_x8)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_x0;
    *tid = (cloudabi_tid_t)reg_x1;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_proc_raise(cloudabi_signal_t sig) {
  register uint64_t reg_x8 asm("x8") = 43;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sig;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_random_get(void *buf, size_t nbyte) {
  register uint64_t reg_x8 asm("x8") = 44;
  register uint64_t reg_x0 asm("x0") = (uint64_t)buf;
  register uint64_t reg_x1 asm("x1") = (uint64_t)nbyte;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_accept(cloudabi_fd_t sock,
                                                     cloudabi_sockstat_t *buf,
                                                     cloudabi_fd_t *conn) {
  register uint64_t reg_x8 asm("x8") = 45;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)buf;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *conn = (cloudabi_fd_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_bind(cloudabi_fd_t sock,
                                                   cloudabi_fd_t fd,
                                                   const char *path,
                                                   size_t pathlen) {
  register uint64_t reg_x8 asm("x8") = 46;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)fd;
  register uint64_t reg_x2 asm("x2") = (uint64_t)path;
  register uint64_t reg_x3 asm("x3") = (uint64_t)pathlen;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_connect(cloudabi_fd_t sock,
                                                      cloudabi_fd_t fd,
                                                      const char *path,
                                                      size_t pathlen) {
  register uint64_t reg_x8 asm("x8") = 47;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)fd;
  register uint64_t reg_x2 asm("x2") = (uint64_t)path;
  register uint64_t reg_x3 asm("x3") = (uint64_t)pathlen;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2), "r"(reg_x3)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_listen(
    cloudabi_fd_t sock, cloudabi_backlog_t backlog) {
  register uint64_t reg_x8 asm("x8") = 48;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)backlog;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_recv(cloudabi_fd_t sock,
                                                   const cloudabi_recv_in_t *in,
                                                   cloudabi_recv_out_t *out) {
  register uint64_t reg_x8 asm("x8") = 49;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)in;
  register uint64_t reg_x2 asm("x2") = (uint64_t)out;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_send(cloudabi_fd_t sock,
                                                   const cloudabi_send_in_t *in,
                                                   cloudabi_send_out_t *out) {
  register uint64_t reg_x8 asm("x8") = 50;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)in;
  register uint64_t reg_x2 asm("x2") = (uint64_t)out;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_shutdown(cloudabi_fd_t sock,
                                                       cloudabi_sdflags_t how) {
  register uint64_t reg_x8 asm("x8") = 51;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)how;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_sock_stat_get(
    cloudabi_fd_t sock, cloudabi_sockstat_t *buf, cloudabi_ssflags_t flags) {
  register uint64_t reg_x8 asm("x8") = 52;
  register uint64_t reg_x0 asm("x0") = (uint64_t)sock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)buf;
  register uint64_t reg_x2 asm("x2") = (uint64_t)flags;
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1), "r"(reg_x2)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

static cloudabi_errno_t syscall_fallback_thread_create(
    cloudabi_threadattr_t *attr, cloudabi_tid_t *tid) {
  register uint64_t reg_x8 asm("x8") = 53;
  register uint64_t reg_x0 asm("x0") = (uint64_t)attr;
  register uint64_t okay;
  asm volatile(
      "\tsvc 0\n"
      "\tcset %0, cc\n"
      : "=r"(okay), "=r"(reg_x0)
      : "r"(reg_x8), "r"(reg_x0)
      : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
        "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0",
        "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  if (okay) {
    *tid = (cloudabi_tid_t)reg_x0;
    return 0;
  }
  return reg_x0;
}

static noreturn void syscall_fallback_thread_exit(_Atomic(cloudabi_lock_t) *
                                                      lock,
                                                  cloudabi_scope_t scope) {
  register uint64_t reg_x8 asm("x8") = 54;
  register uint64_t reg_x0 asm("x0") = (uint64_t)lock;
  register uint64_t reg_x1 asm("x1") = (uint64_t)scope;
  asm volatile("\tsvc 0\n"
               :
               : "r"(reg_x8), "r"(reg_x0), "r"(reg_x1)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  for (;;)
    ;
}

static cloudabi_errno_t syscall_fallback_thread_yield(void) {
  register uint64_t reg_x8 asm("x8") = 56;
  register uint64_t reg_x0 asm("x0");
  asm volatile("\tsvc 0\n"
               : "=r"(reg_x0)
               : "r"(reg_x8)
               : "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
                 "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17",
                 "x18", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7");
  return reg_x0;
}

#elif defined(__x86_64__)

static cloudabi_errno_t syscall_fallback_clock_res_get(
    cloudabi_clockid_t clock_id, cloudabi_timestamp_t *resolution) {
  register uint64_t reg_rax asm("rax") = 0;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)clock_id;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *resolution = (cloudabi_timestamp_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_clock_time_get(
    cloudabi_clockid_t clock_id, cloudabi_timestamp_t precision,
    cloudabi_timestamp_t *time) {
  register uint64_t reg_rax asm("rax") = 1;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)clock_id;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)precision;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *time = (cloudabi_timestamp_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_condvar_signal(
    _Atomic(cloudabi_condvar_t) * condvar, cloudabi_scope_t scope,
    cloudabi_nthreads_t nwaiters) {
  register uint64_t reg_rax asm("rax") = 2;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)condvar;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)scope;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)nwaiters;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_close(cloudabi_fd_t fd) {
  register uint64_t reg_rax asm("rax") = 3;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_create1(cloudabi_filetype_t type,
                                                    cloudabi_fd_t *fd) {
  register uint64_t reg_rax asm("rax") = 4;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)type;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_create2(cloudabi_filetype_t type,
                                                    cloudabi_fd_t *fd1,
                                                    cloudabi_fd_t *fd2) {
  register uint64_t reg_rax asm("rax") = 5;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)type;
  register uint64_t reg_rdx asm("rdx");
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax), "=r"(reg_rdx)
      : "r"(reg_rax), "r"(reg_rdi)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *fd1 = (cloudabi_fd_t)reg_rax;
    *fd2 = (cloudabi_fd_t)reg_rdx;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_datasync(cloudabi_fd_t fd) {
  register uint64_t reg_rax asm("rax") = 6;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_dup(cloudabi_fd_t from,
                                                cloudabi_fd_t *fd) {
  register uint64_t reg_rax asm("rax") = 7;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)from;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_pread(cloudabi_fd_t fd,
                                                  const cloudabi_iovec_t *iov,
                                                  size_t iovcnt,
                                                  cloudabi_filesize_t offset,
                                                  size_t *nread) {
  register uint64_t reg_rax asm("rax") = 8;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)iov;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)iovcnt;
  register uint64_t reg_r10 asm("r10") = (uint64_t)offset;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx), "r"(reg_r10)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *nread = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_pwrite(cloudabi_fd_t fd,
                                                   const cloudabi_ciovec_t *iov,
                                                   size_t iovcnt,
                                                   cloudabi_filesize_t offset,
                                                   size_t *nwritten) {
  register uint64_t reg_rax asm("rax") = 9;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)iov;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)iovcnt;
  register uint64_t reg_r10 asm("r10") = (uint64_t)offset;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx), "r"(reg_r10)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *nwritten = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_read(cloudabi_fd_t fd,
                                                 const cloudabi_iovec_t *iov,
                                                 size_t iovcnt, size_t *nread) {
  register uint64_t reg_rax asm("rax") = 10;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)iov;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)iovcnt;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *nread = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_replace(cloudabi_fd_t from,
                                                    cloudabi_fd_t to) {
  register uint64_t reg_rax asm("rax") = 11;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)from;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)to;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_seek(
    cloudabi_fd_t fd, cloudabi_filedelta_t offset, cloudabi_whence_t whence,
    cloudabi_filesize_t *newoffset) {
  register uint64_t reg_rax asm("rax") = 12;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)offset;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)whence;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *newoffset = (cloudabi_filesize_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_stat_get(cloudabi_fd_t fd,
                                                     cloudabi_fdstat_t *buf) {
  register uint64_t reg_rax asm("rax") = 13;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)buf;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_stat_put(
    cloudabi_fd_t fd, const cloudabi_fdstat_t *buf, cloudabi_fdsflags_t flags) {
  register uint64_t reg_rax asm("rax") = 14;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)buf;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)flags;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_sync(cloudabi_fd_t fd) {
  register uint64_t reg_rax asm("rax") = 15;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_fd_write(cloudabi_fd_t fd,
                                                  const cloudabi_ciovec_t *iov,
                                                  size_t iovcnt,
                                                  size_t *nwritten) {
  register uint64_t reg_rax asm("rax") = 16;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)iov;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)iovcnt;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *nwritten = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_advise(cloudabi_fd_t fd,
                                                     cloudabi_filesize_t offset,
                                                     cloudabi_filesize_t len,
                                                     cloudabi_advice_t advice) {
  register uint64_t reg_rax asm("rax") = 17;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)offset;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)len;
  register uint64_t reg_r10 asm("r10") = (uint64_t)advice;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_allocate(
    cloudabi_fd_t fd, cloudabi_filesize_t offset, cloudabi_filesize_t len) {
  register uint64_t reg_rax asm("rax") = 18;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)offset;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)len;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_create(cloudabi_fd_t fd,
                                                     const char *path,
                                                     size_t pathlen,
                                                     cloudabi_filetype_t type) {
  register uint64_t reg_rax asm("rax") = 19;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)pathlen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)type;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_link(
    cloudabi_lookup_t fd1, const char *path1, size_t path1len,
    cloudabi_fd_t fd2, const char *path2, size_t path2len) {
  register uint64_t reg_rax asm("rax") = 20;
  register uint64_t reg_rdi asm("rdi") = *(uint64_t *)&fd1;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path1;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)path1len;
  register uint64_t reg_r10 asm("r10") = (uint64_t)fd2;
  register uint64_t reg_r8 asm("r8") = (uint64_t)path2;
  register uint64_t reg_r9 asm("r9") = (uint64_t)path2len;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10), "r"(reg_r8), "r"(reg_r9)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_open(
    cloudabi_lookup_t dirfd, const char *path, size_t pathlen,
    cloudabi_oflags_t oflags, const cloudabi_fdstat_t *fds, cloudabi_fd_t *fd) {
  register uint64_t reg_rax asm("rax") = 21;
  register uint64_t reg_rdi asm("rdi") = *(uint64_t *)&dirfd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)pathlen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)oflags;
  register uint64_t reg_r8 asm("r8") = (uint64_t)fds;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx), "r"(reg_r10),
        "r"(reg_r8)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_readdir(
    cloudabi_fd_t fd, void *buf, size_t nbyte, cloudabi_dircookie_t cookie,
    size_t *bufused) {
  register uint64_t reg_rax asm("rax") = 22;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)buf;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)nbyte;
  register uint64_t reg_r10 asm("r10") = (uint64_t)cookie;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx), "r"(reg_r10)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *bufused = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_readlink(
    cloudabi_fd_t fd, const char *path, size_t pathlen, char *buf,
    size_t bufsize, size_t *bufused) {
  register uint64_t reg_rax asm("rax") = 23;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)pathlen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)buf;
  register uint64_t reg_r8 asm("r8") = (uint64_t)bufsize;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx), "r"(reg_r10),
        "r"(reg_r8)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *bufused = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_rename(
    cloudabi_fd_t oldfd, const char *old, size_t oldlen, cloudabi_fd_t newfd,
    const char *new, size_t newlen) {
  register uint64_t reg_rax asm("rax") = 24;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)oldfd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)old;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)oldlen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)newfd;
  register uint64_t reg_r8 asm("r8") = (uint64_t) new;
  register uint64_t reg_r9 asm("r9") = (uint64_t)newlen;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10), "r"(reg_r8), "r"(reg_r9)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_stat_fget(
    cloudabi_fd_t fd, cloudabi_filestat_t *buf) {
  register uint64_t reg_rax asm("rax") = 25;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)buf;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_stat_fput(
    cloudabi_fd_t fd, const cloudabi_filestat_t *buf,
    cloudabi_fsflags_t flags) {
  register uint64_t reg_rax asm("rax") = 26;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)buf;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)flags;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_stat_get(
    cloudabi_lookup_t fd, const char *path, size_t pathlen,
    cloudabi_filestat_t *buf) {
  register uint64_t reg_rax asm("rax") = 27;
  register uint64_t reg_rdi asm("rdi") = *(uint64_t *)&fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)pathlen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)buf;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_stat_put(
    cloudabi_lookup_t fd, const char *path, size_t pathlen,
    const cloudabi_filestat_t *buf, cloudabi_fsflags_t flags) {
  register uint64_t reg_rax asm("rax") = 28;
  register uint64_t reg_rdi asm("rdi") = *(uint64_t *)&fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)pathlen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)buf;
  register uint64_t reg_r8 asm("r8") = (uint64_t)flags;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10), "r"(reg_r8)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_symlink(const char *path1,
                                                      size_t path1len,
                                                      cloudabi_fd_t fd,
                                                      const char *path2,
                                                      size_t path2len) {
  register uint64_t reg_rax asm("rax") = 29;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)path1;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path1len;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)fd;
  register uint64_t reg_r10 asm("r10") = (uint64_t)path2;
  register uint64_t reg_r8 asm("r8") = (uint64_t)path2len;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10), "r"(reg_r8)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_file_unlink(cloudabi_fd_t fd,
                                                     const char *path,
                                                     size_t pathlen,
                                                     cloudabi_ulflags_t flags) {
  register uint64_t reg_rax asm("rax") = 30;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)path;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)pathlen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)flags;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_lock_unlock(_Atomic(cloudabi_lock_t) *
                                                         lock,
                                                     cloudabi_scope_t scope) {
  register uint64_t reg_rax asm("rax") = 31;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)lock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)scope;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_mem_advise(void *addr, size_t len,
                                                    cloudabi_advice_t advice) {
  register uint64_t reg_rax asm("rax") = 32;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)addr;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)len;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)advice;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_mem_lock(const void *addr,
                                                  size_t len) {
  register uint64_t reg_rax asm("rax") = 33;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)addr;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)len;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_mem_map(
    void *addr, size_t len, cloudabi_mprot_t prot, cloudabi_mflags_t flags,
    cloudabi_fd_t fd, cloudabi_filesize_t off, void **mem) {
  register uint64_t reg_rax asm("rax") = 34;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)addr;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)len;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)prot;
  register uint64_t reg_r10 asm("r10") = (uint64_t)flags;
  register uint64_t reg_r8 asm("r8") = (uint64_t)fd;
  register uint64_t reg_r9 asm("r9") = (uint64_t)off;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx), "r"(reg_r10),
        "r"(reg_r8), "r"(reg_r9)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *mem = (void *)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_mem_protect(void *addr, size_t len,
                                                     cloudabi_mprot_t prot) {
  register uint64_t reg_rax asm("rax") = 35;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)addr;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)len;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)prot;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_mem_sync(void *addr, size_t len,
                                                  cloudabi_msflags_t flags) {
  register uint64_t reg_rax asm("rax") = 36;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)addr;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)len;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)flags;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_mem_unlock(const void *addr,
                                                    size_t len) {
  register uint64_t reg_rax asm("rax") = 37;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)addr;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)len;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_mem_unmap(void *addr, size_t len) {
  register uint64_t reg_rax asm("rax") = 38;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)addr;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)len;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_poll(const cloudabi_subscription_t *in,
                                              cloudabi_event_t *out,
                                              size_t nsubscriptions,
                                              size_t *nevents) {
  register uint64_t reg_rax asm("rax") = 39;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)in;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)out;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)nsubscriptions;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *nevents = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_poll_fd(
    cloudabi_fd_t fd, const cloudabi_subscription_t *in, size_t nin,
    cloudabi_event_t *out, size_t nout, const cloudabi_subscription_t *timeout,
    size_t *nevents) {
  register uint64_t reg_rax asm("rax") = 57;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)in;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)nin;
  register uint64_t reg_r10 asm("r10") = (uint64_t)out;
  register uint64_t reg_r8 asm("r8") = (uint64_t)nout;
  register uint64_t reg_r9 asm("r9") = (uint64_t)timeout;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx), "r"(reg_r10),
        "r"(reg_r8), "r"(reg_r9)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *nevents = (size_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_proc_exec(cloudabi_fd_t fd,
                                                   const void *data,
                                                   size_t datalen,
                                                   const cloudabi_fd_t *fds,
                                                   size_t fdslen) {
  register uint64_t reg_rax asm("rax") = 40;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)fd;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)data;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)datalen;
  register uint64_t reg_r10 asm("r10") = (uint64_t)fds;
  register uint64_t reg_r8 asm("r8") = (uint64_t)fdslen;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10), "r"(reg_r8)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static noreturn void syscall_fallback_proc_exit(cloudabi_exitcode_t rval) {
  register uint64_t reg_rax asm("rax") = 41;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)rval;
  asm volatile("\tsyscall\n"
               :
               : "r"(reg_rax), "r"(reg_rdi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  for (;;)
    ;
}

static cloudabi_errno_t syscall_fallback_proc_fork(cloudabi_fd_t *fd,
                                                   cloudabi_tid_t *tid) {
  register uint64_t reg_rax asm("rax") = 42;
  register uint64_t reg_rdx asm("rdx");
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax), "=r"(reg_rdx)
      : "r"(reg_rax)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *fd = (cloudabi_fd_t)reg_rax;
    *tid = (cloudabi_tid_t)reg_rdx;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_proc_raise(cloudabi_signal_t sig) {
  register uint64_t reg_rax asm("rax") = 43;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sig;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_random_get(void *buf, size_t nbyte) {
  register uint64_t reg_rax asm("rax") = 44;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)buf;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)nbyte;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_accept(cloudabi_fd_t sock,
                                                     cloudabi_sockstat_t *buf,
                                                     cloudabi_fd_t *conn) {
  register uint64_t reg_rax asm("rax") = 45;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)buf;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *conn = (cloudabi_fd_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_bind(cloudabi_fd_t sock,
                                                   cloudabi_fd_t fd,
                                                   const char *path,
                                                   size_t pathlen) {
  register uint64_t reg_rax asm("rax") = 46;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)fd;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)path;
  register uint64_t reg_r10 asm("r10") = (uint64_t)pathlen;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_connect(cloudabi_fd_t sock,
                                                      cloudabi_fd_t fd,
                                                      const char *path,
                                                      size_t pathlen) {
  register uint64_t reg_rax asm("rax") = 47;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)fd;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)path;
  register uint64_t reg_r10 asm("r10") = (uint64_t)pathlen;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx),
                 "r"(reg_r10)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_listen(
    cloudabi_fd_t sock, cloudabi_backlog_t backlog) {
  register uint64_t reg_rax asm("rax") = 48;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)backlog;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_recv(cloudabi_fd_t sock,
                                                   const cloudabi_recv_in_t *in,
                                                   cloudabi_recv_out_t *out) {
  register uint64_t reg_rax asm("rax") = 49;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)in;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)out;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_send(cloudabi_fd_t sock,
                                                   const cloudabi_send_in_t *in,
                                                   cloudabi_send_out_t *out) {
  register uint64_t reg_rax asm("rax") = 50;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)in;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)out;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_shutdown(cloudabi_fd_t sock,
                                                       cloudabi_sdflags_t how) {
  register uint64_t reg_rax asm("rax") = 51;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)how;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_sock_stat_get(
    cloudabi_fd_t sock, cloudabi_sockstat_t *buf, cloudabi_ssflags_t flags) {
  register uint64_t reg_rax asm("rax") = 52;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)sock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)buf;
  register uint64_t reg_rdx asm("rdx") = (uint64_t)flags;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi), "r"(reg_rdx)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

static cloudabi_errno_t syscall_fallback_thread_create(
    cloudabi_threadattr_t *attr, cloudabi_tid_t *tid) {
  register uint64_t reg_rax asm("rax") = 53;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)attr;
  register char okay;
  asm volatile(
      "\tsyscall\n"
      "\tsetnc %0\n"
      : "=r"(okay), "=r"(reg_rax)
      : "r"(reg_rax), "r"(reg_rdi)
      : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  if (okay) {
    *tid = (cloudabi_tid_t)reg_rax;
    return 0;
  }
  return reg_rax;
}

static noreturn void syscall_fallback_thread_exit(_Atomic(cloudabi_lock_t) *
                                                      lock,
                                                  cloudabi_scope_t scope) {
  register uint64_t reg_rax asm("rax") = 54;
  register uint64_t reg_rdi asm("rdi") = (uint64_t)lock;
  register uint64_t reg_rsi asm("rsi") = (uint64_t)scope;
  asm volatile("\tsyscall\n"
               :
               : "r"(reg_rax), "r"(reg_rdi), "r"(reg_rsi)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  for (;;)
    ;
}

static cloudabi_errno_t syscall_fallback_thread_yield(void) {
  register uint64_t reg_rax asm("rax") = 56;
  asm volatile("\tsyscall\n"
               : "=r"(reg_rax)
               : "r"(reg_rax)
               : "memory", "rcx", "rdx", "r8", "r9", "r10", "r11");
  return reg_rax;
}

#endif

static void syscall_fallback_setup(cloudabi_syscalls_t *syscalls) {
#if defined(__aarch64__) || defined(__x86_64__)
  syscalls->clock_res_get = syscall_fallback_clock_res_get;
  syscalls->clock_time_get = syscall_fallback_clock_time_get;
  syscalls->condvar_signal = syscall_fallback_condvar_signal;
  syscalls->fd_close = syscall_fallback_fd_close;
  syscalls->fd_create1 = syscall_fallback_fd_create1;
  syscalls->fd_create2 = syscall_fallback_fd_create2;
  syscalls->fd_datasync = syscall_fallback_fd_datasync;
  syscalls->fd_dup = syscall_fallback_fd_dup;
  syscalls->fd_pread = syscall_fallback_fd_pread;
  syscalls->fd_pwrite = syscall_fallback_fd_pwrite;
  syscalls->fd_read = syscall_fallback_fd_read;
  syscalls->fd_replace = syscall_fallback_fd_replace;
  syscalls->fd_seek = syscall_fallback_fd_seek;
  syscalls->fd_stat_get = syscall_fallback_fd_stat_get;
  syscalls->fd_stat_put = syscall_fallback_fd_stat_put;
  syscalls->fd_sync = syscall_fallback_fd_sync;
  syscalls->fd_write = syscall_fallback_fd_write;
  syscalls->file_advise = syscall_fallback_file_advise;
  syscalls->file_allocate = syscall_fallback_file_allocate;
  syscalls->file_create = syscall_fallback_file_create;
  syscalls->file_link = syscall_fallback_file_link;
  syscalls->file_open = syscall_fallback_file_open;
  syscalls->file_readdir = syscall_fallback_file_readdir;
  syscalls->file_readlink = syscall_fallback_file_readlink;
  syscalls->file_rename = syscall_fallback_file_rename;
  syscalls->file_stat_fget = syscall_fallback_file_stat_fget;
  syscalls->file_stat_fput = syscall_fallback_file_stat_fput;
  syscalls->file_stat_get = syscall_fallback_file_stat_get;
  syscalls->file_stat_put = syscall_fallback_file_stat_put;
  syscalls->file_symlink = syscall_fallback_file_symlink;
  syscalls->file_unlink = syscall_fallback_file_unlink;
  syscalls->lock_unlock = syscall_fallback_lock_unlock;
  syscalls->mem_advise = syscall_fallback_mem_advise;
  syscalls->mem_lock = syscall_fallback_mem_lock;
  syscalls->mem_map = syscall_fallback_mem_map;
  syscalls->mem_protect = syscall_fallback_mem_protect;
  syscalls->mem_sync = syscall_fallback_mem_sync;
  syscalls->mem_unlock = syscall_fallback_mem_unlock;
  syscalls->mem_unmap = syscall_fallback_mem_unmap;
  syscalls->poll = syscall_fallback_poll;
  syscalls->poll_fd = syscall_fallback_poll_fd;
  syscalls->proc_exec = syscall_fallback_proc_exec;
  syscalls->proc_exit = syscall_fallback_proc_exit;
  syscalls->proc_fork = syscall_fallback_proc_fork;
  syscalls->proc_raise = syscall_fallback_proc_raise;
  syscalls->random_get = syscall_fallback_random_get;
  syscalls->sock_accept = syscall_fallback_sock_accept;
  syscalls->sock_bind = syscall_fallback_sock_bind;
  syscalls->sock_connect = syscall_fallback_sock_connect;
  syscalls->sock_listen = syscall_fallback_sock_listen;
  syscalls->sock_recv = syscall_fallback_sock_recv;
  syscalls->sock_send = syscall_fallback_sock_send;
  syscalls->sock_shutdown = syscall_fallback_sock_shutdown;
  syscalls->sock_stat_get = syscall_fallback_sock_stat_get;
  syscalls->thread_create = syscall_fallback_thread_create;
  syscalls->thread_exit = syscall_fallback_thread_exit;
  syscalls->thread_yield = syscall_fallback_thread_yield;
#endif
}

#endif
