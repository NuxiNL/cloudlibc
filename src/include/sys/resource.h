// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <sys/resource.h> - definitions for XSI resource operations
//
// Features missing:
// - PRIO_PROCESS, PRIO_PGRP, PRIO_USER, getpriority(), setpriority()
//   and id_t:
//   Scheduler interaction not available.
// - rlim_t, RLIM_INFINITY, RLIM_SAVED_MAX, RLIM_SAVED_CUR,
//   struct rlimit, RLIMIT_CORE, RLIMIT_CPU, RLIMIT_DATA, RLIMIT_FSIZE,
//   RLIMIT_NOFILE, RLIMIT_STACK, RLIMIT_AS, getrlimit() and setrlimit():
//   Resource limiting not available.

#ifndef _SYS_RESOURCE_H_
#define _SYS_RESOURCE_H_

#include <_/cdefs.h>
#include <_/struct/timeval.h>  // IWYU pragma: export

// Target specifiers for getrusage().
#define RUSAGE_SELF 1      // Returns information about the current process.
#define RUSAGE_CHILDREN 2  // Returns information about children of the process.

struct rusage {
  struct timeval ru_utime;  // User time used.
  struct timeval ru_stime;  // System time used.
};

__BEGIN_DECLS
int getrusage(int, struct rusage *);
__END_DECLS

#endif
