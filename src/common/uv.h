// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_UV_H
#define COMMON_UV_H

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define _UV_SLIST_DECLARE_FUNCTIONS(name, type)                      \
  static inline void name##_init(struct name##_head *head) {         \
    head->__first = NULL;                                            \
  }                                                                  \
                                                                     \
  static inline bool name##_empty(const struct name##_head *head) {  \
    return head->__first == NULL;                                    \
  }                                                                  \
                                                                     \
  static inline type *name##_first(const struct name##_head *head) { \
    return head->__first;                                            \
  }                                                                  \
                                                                     \
  static inline void name##_insert_first(struct name##_head *head,   \
                                         type *element) {            \
    element->name##_entry.__next = head->__first;                    \
    head->__first = element;                                         \
  }                                                                  \
                                                                     \
  static inline void name##_remove_first(struct name##_head *head) { \
    head->__first = head->__first->name##_entry.__next;              \
  }                                                                  \
                                                                     \
  static inline void name##_move(struct name##_head *from,           \
                                 struct name##_head *to) {           \
    *to = *from;                                                     \
    name##_init(from);                                               \
  }

#define _UV_TAILQ_DECLARE_FUNCTIONS(name, type)                           \
  static inline void name##_init(struct name##_head *head) {              \
    struct __uv_queue_link *headl = &head->__link;                        \
    headl->__next = headl;                                                \
    headl->__prev = headl;                                                \
  }                                                                       \
                                                                          \
  static inline bool name##_empty(const struct name##_head *head) {       \
    const struct __uv_queue_link *headl = &head->__link;                  \
    return headl->__next == headl;                                        \
  }                                                                       \
                                                                          \
  static inline type *name##_first(const struct name##_head *head) {      \
    assert(!name##_empty(head) && "Empty list has no first element");     \
    return __containerof(head->__link.__next, type, name##_entry.__link); \
  }                                                                       \
                                                                          \
  static inline type *name##_last(const struct name##_head *head) {       \
    assert(!name##_empty(head) && "Empty list has no first element");     \
    return __containerof(head->__link.__prev, type, name##_entry.__link); \
  }                                                                       \
                                                                          \
  static inline type *name##_next(const type *element) {                  \
    return __containerof(element->name##_entry.__link.__next, type,       \
                         name##_entry.__link);                            \
  }                                                                       \
                                                                          \
  static inline void name##_insert_last(struct name##_head *head,         \
                                        type *element) {                  \
    struct __uv_queue_link *headl = &head->__link;                        \
    struct __uv_queue_link *elementl = &element->name##_entry.__link;     \
    elementl->__next = headl;                                             \
    elementl->__prev = headl->__prev;                                     \
    headl->__prev->__next = elementl;                                     \
    headl->__prev = elementl;                                             \
  }                                                                       \
                                                                          \
  static inline void name##_remove(type *element) {                       \
    struct __uv_queue_link *elementl = &element->name##_entry.__link;     \
    elementl->__next->__prev = elementl->__prev;                          \
    elementl->__prev->__next = elementl->__next;                          \
  }                                                                       \
                                                                          \
  static inline void name##_move(struct name##_head *from,                \
                                 struct name##_head *to) {                \
    if (name##_empty(from)) {                                             \
      name##_init(to);                                                    \
    } else {                                                              \
      struct __uv_queue_link *froml = &from->__link;                      \
      struct __uv_queue_link *tol = &to->__link;                          \
      *tol = *froml;                                                      \
      tol->__next->__prev = tol;                                          \
      tol->__prev->__next = tol;                                          \
      name##_init(from);                                                  \
    }                                                                     \
  }

#define _UV_HEAP_DECLARE_FUNCTIONS(name, type)                                 \
  static inline type *name##_min(const struct name##_head *heap) {             \
    return heap->__length == 0 ? NULL : heap->__elements[0];                   \
  }                                                                            \
                                                                               \
  static inline void name##_init(struct name##_head *heap) {                   \
    heap->__elements = NULL;                                                   \
    heap->__length = 0;                                                        \
    heap->__capacity = 0;                                                      \
    heap->__next_sequence = 0;                                                 \
  }                                                                            \
                                                                               \
  static inline void name##_destroy(struct name##_head *heap) {                \
    free(heap->__elements);                                                    \
  }                                                                            \
                                                                               \
  static inline bool name##_lt(const type *a, const type *b) {                 \
    int cmp = name##_cmp(a, b);                                                \
    if (cmp < 0)                                                               \
      return true;                                                             \
    if (cmp > 0)                                                               \
      return false;                                                            \
    /* Ensure determinism by respecting insertion order. */                    \
    return a->name##_entry.__sequence < b->name##_entry.__sequence;            \
  }                                                                            \
                                                                               \
  static inline void name##_swap(struct name##_head *heap, size_t a,           \
                                 size_t b) {                                   \
    type *tmp = heap->__elements[a];                                           \
    heap->__elements[a] = heap->__elements[b];                                 \
    heap->__elements[b] = tmp;                                                 \
    heap->__elements[a]->name##_entry.__index = a;                             \
    heap->__elements[b]->name##_entry.__index = b;                             \
  }                                                                            \
                                                                               \
  static inline void name##_decreased(struct name##_head *heap,                \
                                      type *element) {                         \
    size_t child = element->name##_entry.__index;                              \
    while (child > 0) {                                                        \
      size_t parent = (child - 1) / 2;                                         \
      if (name##_lt(heap->__elements[parent], heap->__elements[child]))        \
        break;                                                                 \
      name##_swap(heap, parent, child);                                        \
      child = parent;                                                          \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void name##_increased(struct name##_head *heap,                \
                                      type *element) {                         \
    size_t top = element->name##_entry.__index;                                \
    for (;;) {                                                                 \
      size_t smallest = top;                                                   \
      size_t left = top * 2 + 1;                                               \
      size_t right = top * 2 + 2;                                              \
      if (left < heap->__length &&                                             \
          name##_lt(heap->__elements[left], heap->__elements[smallest]))       \
        smallest = left;                                                       \
      if (right < heap->__length &&                                            \
          name##_lt(heap->__elements[right], heap->__elements[smallest]))      \
        smallest = right;                                                      \
      if (smallest == top)                                                     \
        break;                                                                 \
      name##_swap(heap, top, smallest);                                        \
      top = smallest;                                                          \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void name##_changed(struct name##_head *heap, type *element) { \
    name##_decreased(heap, element);                                           \
    name##_increased(heap, element);                                           \
  }                                                                            \
                                                                               \
  static inline bool name##_insert(struct name##_head *heap, type *element) {  \
    if (heap->__length == heap->__capacity) {                                  \
      size_t new_capacity = heap->__capacity < 4 ? 4 : heap->__capacity * 2;   \
      type **new_elements =                                                    \
          reallocarray(heap->__elements, new_capacity, sizeof(type *));        \
      if (new_elements == NULL)                                                \
        return false;                                                          \
      heap->__elements = new_elements;                                         \
      heap->__capacity = new_capacity;                                         \
    }                                                                          \
    size_t index = heap->__length++;                                           \
    heap->__elements[index] = element;                                         \
    element->name##_entry.__index = index;                                     \
    name##_decreased(heap, element);                                           \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline void name##_remove_min(struct name##_head *heap) {             \
    type *root = heap->__elements[--heap->__length];                           \
    heap->__elements[0] = root;                                                \
    root->name##_entry.__index = 0;                                            \
    name##_increased(heap, root);                                              \
  }                                                                            \
                                                                               \
  static inline void name##_remove(struct name##_head *heap, type *element) {  \
    size_t index = element->name##_entry.__index;                              \
    type *root = heap->__elements[--heap->__length];                           \
    heap->__elements[index] = root;                                            \
    root->name##_entry.__index = index;                                        \
    name##_changed(heap, root);                                                \
  }

#define _UV_CIRCLEBUF_DECLARE_FUNCTIONS(name, type)                            \
  static inline void name##_init(struct name##_head *head) {                   \
    head->__elements = NULL;                                                   \
    head->__offset = 0;                                                        \
    head->__length = 0;                                                        \
    head->__capacity = 0;                                                      \
  }                                                                            \
                                                                               \
  static inline size_t name##_count(struct name##_head *head) {                \
    return head->__length;                                                     \
  }                                                                            \
                                                                               \
  static inline bool name##_empty(struct name##_head *head) {                  \
    return name##_count(head) == 0;                                            \
  }                                                                            \
                                                                               \
  static inline void name##_destroy(struct name##_head *head) {                \
    assert(name##_empty(head) && "Attempted to destroy a used buffer");        \
    free(head->__elements);                                                    \
  }                                                                            \
                                                                               \
  static inline type name##_first(struct name##_head *head) {                  \
    assert(!name##_empty(head) && "Attempted to access empty buffer");         \
    return head->__elements[head->__offset];                                   \
  }                                                                            \
                                                                               \
  static inline void name##_remove_first(struct name##_head *head) {           \
    assert(!name##_empty(head) && "Attempted to access empty buffer");         \
    head->__offset = (head->__offset + 1) % head->__capacity;                  \
    --head->__length;                                                          \
  }                                                                            \
  static inline bool name##_insert_last(struct name##_head *head,              \
                                        type element) {                        \
    if (head->__length == head->__capacity) {                                  \
      /* Capacity reached. Resize the buffer. */                               \
      size_t new_capacity = head->__capacity < 4 ? 4 : head->__capacity * 2;   \
      type *new_elements =                                                     \
          reallocarray(head->__elements, new_capacity, sizeof(type));          \
      if (new_elements == NULL)                                                \
        return false;                                                          \
      if (head->__offset + head->__length > head->__capacity) {                \
        /* Old contents wrapped around. Restore the wrapping. */               \
        size_t trailing_elements = head->__capacity - head->__offset;          \
        size_t new_offset = new_capacity - trailing_elements;                  \
        memmove(new_elements + new_offset, new_elements + head->__offset,      \
                trailing_elements * sizeof(type));                             \
        head->__offset = new_offset;                                           \
      }                                                                        \
      head->__elements = new_elements;                                         \
      head->__capacity = new_capacity;                                         \
    }                                                                          \
    head->__elements[(head->__offset + head->__length++) % head->__capacity] = \
        element;                                                               \
    return true;                                                               \
  }

static inline int __uv_active_timers_cmp(const uv_timer_t *a,
                                         const uv_timer_t *b) {
  if (a->__timeout < b->__timeout)
    return -1;
  if (a->__timeout > b->__timeout)
    return 1;
  return 0;
}

_UV_CIRCLEBUF_DECLARE_FUNCTIONS(__uv_pending_fds, int);
_UV_HEAP_DECLARE_FUNCTIONS(__uv_active_timers, uv_timer_t);
_UV_SLIST_DECLARE_FUNCTIONS(__uv_closing_handles, uv_handle_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_active_asyncs, uv_async_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_active_checks, uv_check_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_active_idles, uv_idle_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_active_prepares, uv_prepare_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_active_processes, uv_process_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_handles, uv_handle_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_reading_polls, uv_poll_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_reading_streams, uv_stream_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_shutdowns, uv_shutdown_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_writes, uv_write_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_writing_polls, uv_poll_t);
_UV_TAILQ_DECLARE_FUNCTIONS(__uv_writing_streams, uv_stream_t);

//
// Error handling.
//

const char *__uv_strerror_unknown(int);

//
// Handle management.
//

#define UV_HANDLE_ACTIVE 0x1
#define UV_HANDLE_CLOSING 0x2
#define UV_HANDLE_REFERENCED 0x4

static inline int __uv_is_active(const uv_handle_t *handle) {
  assert((handle->__state & (UV_HANDLE_ACTIVE | UV_HANDLE_CLOSING)) !=
             (UV_HANDLE_ACTIVE | UV_HANDLE_CLOSING) &&
         "Handle cannot be active and closing at the same time");
  return (handle->__state & UV_HANDLE_ACTIVE) != 0;
}
#define uv_is_active(handle) __uv_is_active(handle)

static inline int __uv_is_closing(const uv_handle_t *handle) {
  assert((handle->__state & (UV_HANDLE_ACTIVE | UV_HANDLE_CLOSING)) !=
             (UV_HANDLE_ACTIVE | UV_HANDLE_CLOSING) &&
         "Handle cannot be active and closing at the same time");
  return (handle->__state & UV_HANDLE_CLOSING) != 0;
}
#define uv_is_closing(handle) __uv_is_closing(handle)

static inline int __uv_has_ref(const uv_handle_t *handle) {
  return (handle->__state & UV_HANDLE_REFERENCED) != 0;
}
#define uv_has_ref(handle) __uv_has_ref(handle)

static inline void __uv_handle_init(uv_loop_t *loop, uv_handle_t *handle,
                                    uv_handle_type type) {
  handle->loop = loop;
  handle->type = type;
  handle->__state = UV_HANDLE_REFERENCED;
  __uv_handles_insert_last(&loop->__handles, handle);
}

static inline void __uv_handle_start(uv_handle_t *handle) {
  assert(!uv_is_active(handle) && "Cannot start an already active handle");
  assert(!uv_is_closing(handle) && "Cannot start a closing handle");
  handle->__state |= UV_HANDLE_ACTIVE;
  if (uv_has_ref(handle))
    ++handle->loop->__active_ref_handles_reqs;
}

static inline void __uv_handle_stop(uv_handle_t *handle) {
  assert(uv_is_active(handle) && "Cannot stop an already stopped handle");
  assert(!uv_is_closing(handle) && "Cannot stop a closing handle");
  handle->__state &= ~UV_HANDLE_ACTIVE;
  if (uv_has_ref(handle)) {
    uv_loop_t *loop = handle->loop;
    assert(loop->__active_ref_handles_reqs > 0 &&
           "Alive count cannot go negative");
    --loop->__active_ref_handles_reqs;
  }
}

static inline void __uv_stream_init(uv_loop_t *loop, uv_stream_t *handle,
                                    uv_handle_type type) {
  __uv_handle_init(loop, (uv_handle_t *)handle, type);
  handle->write_queue_size = 0;

  handle->__fd = -1;
  handle->__read_cb = NULL;
  __uv_shutdowns_init(&handle->__shutdown_queue);
  __uv_writes_init(&handle->__write_queue);
  __uv_pending_fds_init(&handle->__pending_fds);
}

static inline bool __uv_stream_has_ipc(uv_stream_t *handle) {
  return handle->type == UV_NAMED_PIPE && ((uv_pipe_t *)handle)->ipc != 0;
}

static inline int __uv_stream_open(uv_stream_t *handle, int fd) {
  if (handle->__fd >= 0)
    return UV_EBUSY;

  // Switch the file descriptor to non-blocking mode.
  cloudabi_fdstat_t fds;
  cloudabi_errno_t error = cloudabi_sys_fd_stat_get(fd, &fds);
  if (error != 0)
    return -error;
  fds.fs_flags |= CLOUDABI_FDFLAG_NONBLOCK;
  error = cloudabi_sys_fd_stat_put(fd, &fds, CLOUDABI_FDSTAT_FLAGS);
  if (error != 0)
    return -error;

  handle->__fd = fd;
  return 0;
}

static inline void __uv_stream_start_reading(uv_stream_t *handle) {
  if (handle->__read_cb == NULL) {
    __uv_reading_streams_insert_last(&handle->loop->__reading_streams, handle);
    if (__uv_shutdowns_empty(&handle->__shutdown_queue) &&
        __uv_writes_empty(&handle->__write_queue))
      __uv_handle_start((uv_handle_t *)handle);
  }
}

static inline void __uv_stream_start_writing(uv_stream_t *handle) {
  if (__uv_shutdowns_empty(&handle->__shutdown_queue) &&
      __uv_writes_empty(&handle->__write_queue)) {
    __uv_writing_streams_insert_last(&handle->loop->__writing_streams, handle);
    if (handle->__read_cb == NULL)
      __uv_handle_start((uv_handle_t *)handle);
  }
}

static inline void __uv_stream_stop_reading(uv_stream_t *handle) {
  assert(handle->__read_cb == NULL &&
         "Stream not actually stopped for reading");
  __uv_reading_streams_remove(handle);
  if (__uv_shutdowns_empty(&handle->__shutdown_queue) &&
      __uv_writes_empty(&handle->__write_queue))
    __uv_handle_stop((uv_handle_t *)handle);
}

static inline void __uv_stream_stop_writing(uv_stream_t *handle) {
  if (__uv_shutdowns_empty(&handle->__shutdown_queue) &&
      __uv_writes_empty(&handle->__write_queue)) {
    if (handle->__read_cb == NULL)
      __uv_handle_stop((uv_handle_t *)handle);
    __uv_writing_streams_remove(handle);
  }
}

static inline void __uv_process_kill(uv_process_t *handle) {
  if (handle->__fd >= 0) {
    cloudabi_sys_fd_close(handle->__fd);
    handle->__fd = -1;
  }
}

static inline void __uv_process_stop(uv_process_t *handle) {
  if (uv_is_active((uv_handle_t *)handle)) {
    __uv_process_kill(handle);
    __uv_active_processes_remove(handle);
    __uv_handle_stop((uv_handle_t *)handle);
  }
}

//
// Request management.
//

static inline void __uv_req_init(uv_req_t *req, uv_req_type type) {
  req->type = type;
}

//
// File system operations.
//

static inline int __uv_fs_execute(uv_loop_t *loop, uv_fs_t *req,
                                  uv_fs_type type,
                                  ssize_t (*work_cb)(uv_fs_t *),
                                  uv_fs_cb after_work_cb) {
  // TODO(ed): Implement!
  return UV_ENOSYS;
}

//
// Miscellaneous utilities.
//

static inline void __uv_print_handle(uv_handle_t *handle, FILE *stream) {
  const char *type;
  switch (handle->type) {
#define ENTRY(upper, lower) \
  case UV_##upper:          \
    type = #lower;          \
    break;
    UV_HANDLE_TYPE_MAP(ENTRY)
#undef ENTRY
    default:
      assert(0 && "Unknown type");
  }
  fprintf(stream, "[%c%c-] %-8s %p\n", uv_has_ref(handle) ? 'R' : '-',
          uv_is_active(handle) ? 'A' : '-', type, handle);
}

#endif
