// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_QUEUE_H
#define COMMON_QUEUE_H

#include <stddef.h>

// SLIST: Single-linked list.

#define SLIST_HEAD(name, type) \
  struct name {                \
    struct type *sl_first;     \
  }

#define SLIST_HEAD_INITIALIZER(head) \
  { NULL }

#define SLIST_HEAD_INITIALIZER_NONEMPTY(first) \
  { first }

#define SLIST_ENTRY(type) \
  struct {                \
    struct type *sl_next; \
  }

#define SLIST_ENTRY_INITIALIZER(next) \
  { next }

#define SLIST_EMPTY(head) ((head)->sl_first == NULL)

#define SLIST_FIRST(head) ((head)->sl_first)

#define SLIST_FOREACH(var, head, field) \
  for ((var) = (head)->sl_first; (var) != NULL; (var) = (var)->field.sl_next)

#define SLIST_FOREACH_PREVPTR(var, varprev, head, field)                       \
  for ((var) = (head)->sl_first, (varprev) = &(head)->sl_first; (var) != NULL; \
       (varprev) = *(varprev) == (var) ? &(var)->field.sl_next : (varprev),    \
      (var) = *(varprev))

#define SLIST_FOREACH_SAFE(var, head, field, tmpvar)          \
  for ((var) = (head)->sl_first,                              \
      (tmpvar) = (var) != NULL ? (var)->field.sl_next : NULL; \
       (var) != NULL; (var) = (tmpvar),                       \
      (tmpvar) = (var) != NULL ? (var)->field.sl_next : NULL)

#define SLIST_INSERT_HEAD(head, elm, field)  \
  do {                                       \
    (elm)->field.sl_next = (head)->sl_first; \
    (head)->sl_first = (elm);                \
  } while (0)

#define SLIST_REMOVE_PREVPTR(varprev, field)  \
  do {                                        \
    *(varprev) = (*(varprev))->field.sl_next; \
  } while (0)

#define SLIST_REMOVE_HEAD(head, field)                  \
  do {                                                  \
    (head)->sl_first = (head)->sl_first->field.sl_next; \
  } while (0)

#define SLIST_SWAP(head1, head2, type)     \
  do {                                     \
    struct type *tmp = (head1)->sl_first;  \
    (head1)->sl_first = (head2)->sl_first; \
    (head2)->sl_first = tmp;               \
  } while (0)

// LIST: Double-linked list.

#define LIST_HEAD(name, type) \
  struct name {               \
    struct type *l_first;     \
  }

#define LIST_HEAD_INITIALIZER(head) \
  { NULL }

#define LIST_ENTRY(type)    \
  struct {                  \
    struct type *__l_next;  \
    struct type **__l_prev; \
  }

#define LIST_EMPTY(head) ((head)->l_first == NULL)

#define LIST_INSERT_HEAD(head, element, field)                      \
  do {                                                              \
    (element)->field.__l_next = (head)->l_first;                    \
    if ((head)->l_first != NULL)                                    \
      (head)->l_first->field.__l_prev = &(element)->field.__l_next; \
    (head)->l_first = (element);                                    \
    (element)->field.__l_prev = &(head)->l_first;                   \
  } while (0)

#define LIST_FOREACH(var, head, field) \
  for ((var) = (head)->l_first; (var) != NULL; (var) = (var)->field.__l_next)

#define LIST_FOREACH_SAFE(var, head, field, tmpvar)            \
  for ((var) = (head)->l_first,                                \
      (tmpvar) = (var) != NULL ? (var)->field.__l_next : NULL; \
       (var) != NULL; (var) = (tmpvar),                        \
      (tmpvar) = (var) != NULL ? (var)->field.__l_next : NULL)

#define LIST_REMOVE(element, field)                                          \
  do {                                                                       \
    if ((element)->field.__l_next != NULL)                                   \
      (element)->field.__l_next->field.__l_prev = (element)->field.__l_prev; \
    *(element)->field.__l_prev = (element)->field.__l_next;                  \
  } while (0)

#define LIST_SWAP(head1, head2, type, field)                \
  do {                                                      \
    struct type *tmp = (head1)->l_first;                    \
    (head1)->l_first = (head2)->l_first;                    \
    if ((head1)->l_first != NULL)                           \
      (head1)->l_first->field.__l_prev = &(head1)->l_first; \
    (head2)->l_first = tmp;                                 \
    if ((head2)->l_first != NULL)                           \
      (head2)->l_first->field.__l_prev = &(head2)->l_first; \
  } while (0)

#endif
