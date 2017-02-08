// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
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

// <dlfcn.h> - dynamic linking
//
// Extensions:
// - dladdr():
//   Present on most operating systems. Useful for debugging. Do note
//   that this function only works when executables are linked with
//   "-Wl,--export-dynamic", as it requires the dynamic symbol table.
//
// Features missing:
// - RTLD_*, dlclose(), dlerror(), dlopen() and dlsym():
//   Loading shared libraries is not supported.

#ifndef _DLFCN_H_
#define _DLFCN_H_

#include <_/cdefs.h>

typedef struct {
  const char *dli_fname;  // Pathname of shared object.
  void *dli_fbase;        // Base address of shared object.
  const char *dli_sname;  // Name of nearest symbol.
  void *dli_saddr;        // Address of nearest symbol.
} Dl_info;

__BEGIN_DECLS
int dladdr(const void *, Dl_info *);
__END_DECLS

#endif
