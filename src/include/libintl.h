// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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

// <libintl.h> - iñtërnâtiônàlizætiøn
//
// Features missing:
// - textdomain(), bindtextdomain() and bind_textdomain_codeset():
//   Process wide text domain not available.

#ifndef _LIBINTL_H_
#define _LIBINTL_H_

#include <_/types.h>

// As there is no process wide locale in this environment, the standard
// Gettext interface should always return the input strings literally.

__BEGIN_DECLS
char *dcgettext(const char *, const char *, int);
char *dcngettext(const char *, const char *, const char *, unsigned long, int);
char *dgettext(const char *, const char *);
char *dngettext(const char *, const char *, const char *, unsigned long);
char *gettext(const char *);
char *ngettext(const char *, const char *, unsigned long);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline char *__dcgettext(const char *__domain __unused,
                                  const char *__msgid,
                                  int __category __unused) {
  return (char *)__msgid;
}
#define dcgettext(domain, msgid, category) __dcgettext(domain, msgid, category)

static __inline char *__dcngettext(const char *__domain __unused,
                                   const char *__msgid1, const char *__msgid2,
                                   unsigned long __n, int __category __unused) {
  return (char *)(__n == 1 ? __msgid1 : __msgid2);
}
#define dcngettext(domain, msgid1, msgid2, n, category) \
  __dcngettext(domain, msgid1, msgid2, n, category)

static __inline char *__dgettext(const char *__domain, const char *__msgid) {
  return dcgettext(__domain, __msgid, 2);
}
#define dgettext(domain, msgid) __dgettext(domain, msgid)

static __inline char *__dngettext(const char *__domain, const char *__msgid1,
                                  const char *__msgid2, unsigned long __n) {
  return dcngettext(__domain, __msgid1, __msgid2, __n, 2);
}
#define dngettext(domain, msgid1, msgid2, n) \
  __dngettext(domain, msgid1, msgid2, n)

static __inline char *__gettext(const char *__msgid) {
  return dcgettext(_NULL, __msgid, 2);
}
#define gettext(msgid) __gettext(msgid)

static __inline char *__ngettext(const char *__msgid1, const char *__msgid2,
                                 unsigned long __n) {
  return dcngettext(_NULL, __msgid1, __msgid2, __n, 2);
}
#define ngettext(msgid1, msgid2, n) __ngettext(msgid1, msgid2, n)
#endif

#endif
