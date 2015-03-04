// Copyright (c) 2015 Nuxi, https://nuxi.nl/
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

// <dirent.h> - format of directory entries
//
// Extensions:
// - struct dirent::d_type and DT_*:
//   Present on most operating systems.
// - fdclosedir():
//   Destroys directory object without closing underlying file. Also
//   present on FreeBSD.
// - opendirat():
//   Replacement for opendir().
//
// Features missing:
// - alphasort(), opendir() and scandir():
//   Requires global filesystem namespace.

#ifndef _DIRENT_H_
#define _DIRENT_H_

#include <_/limits.h>
#include <_/types.h>

typedef struct _DIR DIR;
typedef __ino_t ino_t;

struct dirent {
  ino_t d_ino;                 // File serial number.
  int d_type;                  // File type.
  char d_name[_NAME_MAX + 1];  // Filename string of entry.
};

#define DT_UNKNOWN 0      // File type unknown.
#define DT_BLK 0x100000   // Block device.
#define DT_CHR 0x110000   // Character device.
#define DT_DIR 0x200000   // Directory.
#define DT_FIFO 0x300000  // FIFO.
#define DT_LNK 0x900000   // Symbolic link.
#define DT_REG 0x600000   // Regular file.

__BEGIN_DECLS
int closedir(DIR *);
int dirfd(DIR *);
int fdclosedir(DIR *);
DIR *fdopendir(int);
DIR *opendirat(int, const char *);
struct dirent *readdir(DIR *);
int readdir_r(DIR *__restrict, struct dirent *__restrict,
              struct dirent **__restrict);
void rewinddir(DIR *);
void seekdir(DIR *, long);
long telldir(DIR *);
__END_DECLS

#endif
