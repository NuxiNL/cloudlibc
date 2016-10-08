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

// <search.h> - search tables
//
// Extensions:
// - struct hsearch_data, hcreate_r(), hdestroy_r() and hsearch_r():
//   Replacements for hcreate(), hdestroy() and hsearch(). Present on
//   many other systems.
// - posix_tnode:
//   Expected to be part of POSIX issue 8. Unlike POSIX, this
//   implementation defines it as a structure type containing a key
//   pointer. This allows use without potential pointer aliasing.
//   http://austingroupbugs.net/view.php?id=1011
// - tdestroy():
//   Allows for easy destruction of search trees. Also present on Linux.
//
// Features missing:
// - hcreate(), hdestroy() and hsearch():
//   Not thread-safe.

#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <_/types.h>

#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif

typedef enum { FIND, ENTER } ACTION;
typedef enum { preorder, postorder, endorder, leaf } VISIT;

typedef struct {
  char *key;
  void *data;
} ENTRY;

struct __hsearch;

struct hsearch_data {
  struct __hsearch *__hsearch;
};

typedef struct __posix_tnode {
  void *key;                      // Value.
  struct __posix_tnode *__left;   // Left child.
  struct __posix_tnode *__right;  // Right child.
  signed char __balance;          // AVL tree balance, between -1 and 1.
} posix_tnode;

__BEGIN_DECLS
int hcreate_r(size_t, struct hsearch_data *);
void hdestroy_r(struct hsearch_data *);
int hsearch_r(ENTRY, ACTION, ENTRY **, struct hsearch_data *);
void insque(void *, void *);
void *lfind(const void *, const void *, size_t *, size_t,
            int (*)(const void *, const void *));
void *lsearch(const void *, void *, size_t *, size_t,
              int (*)(const void *, const void *));
void remque(void *);
void *tdelete(const void *__restrict, posix_tnode **__restrict,
              int (*)(const void *, const void *));
void tdestroy(posix_tnode *, void (*)(void *));
posix_tnode *tfind(const void *, posix_tnode *const *,
                   int (*)(const void *, const void *));
posix_tnode *tsearch(const void *, posix_tnode **,
                     int (*)(const void *, const void *));
void twalk(const posix_tnode *, void (*)(const posix_tnode *, VISIT, int));
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
struct __que {
  struct __que *__succ;
  struct __que *__pred;
};

static __inline void __insque(void *__element, void *__pred) {
  struct __que *__qelement, *__qsucc, *__qpred;

  __qelement = (struct __que *)__element;
  __qpred = (struct __que *)__pred;
  if (__qpred == _NULL) {
    __qelement->__succ = _NULL;
    __qelement->__pred = _NULL;
  } else {
    __qsucc = __qpred->__succ;
    __qelement->__succ = __qsucc;
    __qelement->__pred = __qpred;
    __qpred->__succ = __qelement;
    if (__qsucc != _NULL)
      __qsucc->__pred = __qelement;
  }
}
#define insque(element, pred) __insque(element, pred)

static __inline void *__lfind(const void *__key, const void *__base,
                              size_t *__nelp, size_t __width,
                              int (*__compar)(const void *, const void *)) {
  const void *__end;

  __end = (void *)((char *)__base + *__nelp * __width);
  while (__base < __end) {
    if (__compar(__key, __base) == 0)
      return (void *)__base;
    __base = (const void *)((const char *)__base + __width);
  }
  return _NULL;
}
#define lfind(key, base, nelp, width, compar) \
  __preserve_const(void, __lfind, base, key, base, nelp, width, compar)

static __inline void *__lsearch(const void *__key, void *__base, size_t *__nelp,
                                size_t __width,
                                int (*__compar)(const void *, const void *)) {
  const void *__end;

  __end = (void *)((char *)__base + *__nelp * __width);
  while (__base < __end) {
    if (__compar(__key, __base) == 0)
      return (void *)__base;
    __base = (void *)((char *)__base + __width);
  }
  __builtin_memcpy(__base, __key, __width);
  ++*__nelp;
  return __base;
}
#define lsearch(key, base, nelp, width, compar) \
  __lsearch(key, base, nelp, width, compar)

static __inline void __remque(void *__element) {
  struct __que *__qelement, *__qsucc, *__qpred;

  __qelement = (struct __que *)__element;
  __qsucc = __qelement->__succ;
  __qpred = __qelement->__pred;
  if (__qsucc != _NULL)
    __qsucc->__pred = __qpred;
  if (__qpred != _NULL)
    __qpred->__succ = __qsucc;
}
#define remque(element) __remque(element)

static __inline posix_tnode *__tfind(const void *__key,
                                     posix_tnode *const *__rootp,
                                     int (*__compar)(const void *,
                                                     const void *)) {
  posix_tnode *__root;
  int __cmp;

  if (__rootp == _NULL)
    return _NULL;
  __root = *__rootp;
  while (__root != _NULL) {
    __cmp = __compar(__key, __root->key);
    if (__cmp < 0)
      __root = __root->__left;
    else if (__cmp > 0)
      __root = __root->__right;
    else
      return __root;
  }
  return _NULL;
}
#define tfind(key, rootp, compar) __tfind(key, rootp, compar)

static __inline void __twalk_recurse(const posix_tnode *__root,
                                     void (*__action)(const posix_tnode *,
                                                      VISIT, int),
                                     int __level) {
  if (__root != _NULL) {
    if (__root->__left == _NULL && __root->__right == _NULL) {
      __action(__root, leaf, __level);
    } else {
      __action(__root, preorder, __level);
      __twalk_recurse(__root->__left, __action, __level + 1);
      __action(__root, postorder, __level);
      __twalk_recurse(__root->__right, __action, __level + 1);
      __action(__root, endorder, __level);
    }
  }
}

static __inline void __twalk(const posix_tnode *__root,
                             void (*__action)(const posix_tnode *, VISIT,
                                              int)) {
  __twalk_recurse(__root, __action, 0);
}
#define twalk(root, action) __twalk(root, action)
#endif

#endif
