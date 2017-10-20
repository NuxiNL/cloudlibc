// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <uv.h>

#include "uv_impl.h"

static void run_timers(uv_loop_t *loop) {
  for (;;) {
    uv_timer_t *timer = __uv_active_timers_min(&loop->__active_timers);
    if (timer == NULL || timer->__timeout > loop->__now)
      break;

    if (timer->__repeat != 0) {
      // Compute new timeout and move it to the right place in the heap.
      timer->__timeout = loop->__now + timer->__repeat;
      __uv_active_timers_increased(&loop->__active_timers, timer);
    } else {
      // One shot timer. Stop it.
      __uv_active_timers_remove_min(&loop->__active_timers);
      __uv_handle_stop((uv_handle_t *)timer);
    }

    timer->__cb(timer);
  }
}

#define GENERATE_RUN_HANDLES(type)                                          \
  static void run_##type##s(uv_loop_t *loop) {                              \
    struct __uv_active_##type##s_head handles;                              \
    __uv_active_##type##s_move(&loop->__active_##type##s, &handles);        \
    while (!__uv_active_##type##s_empty(&handles)) {                        \
      uv_##type##_t *handle = __uv_active_##type##s_first(&handles);        \
      __uv_active_##type##s_remove(handle);                                 \
      __uv_active_##type##s_insert_last(&loop->__active_##type##s, handle); \
      handle->__cb(handle);                                                 \
    }                                                                       \
  }
GENERATE_RUN_HANDLES(idle);
GENERATE_RUN_HANDLES(prepare);
GENERATE_RUN_HANDLES(check);
#undef GENERATE_RUN_HANDLES

static void __uv_async_fd_read(uv_async_t *handle,
                               const cloudabi_event_t *event) {
  // Discard any data present in the pipe used for notification.
  char discard[1024];
  cloudabi_iovec_t iov = {.buf = discard, .buf_len = sizeof(discard)};
  size_t nread;
  while (cloudabi_sys_fd_read(handle->__readfd, &iov, 1, &nread) == 0 &&
         nread == sizeof(discard)) {
  }

  handle->__cb(handle);
}

static void __uv_poll_fd_read(uv_poll_t *handle,
                              const cloudabi_event_t *event) {
  assert((handle->__events & UV_READABLE) != 0 &&
         "Unnecessarily polled handle for reading");
  if (event->error == 0) {
    handle->__revents |= UV_READABLE;
    if ((event->fd_readwrite.flags & CLOUDABI_EVENT_FD_READWRITE_HANGUP) != 0)
      handle->__revents |= UV_DISCONNECT;
  } else {
    handle->__status = -event->error;
  }
}

static void __uv_poll_fd_write(uv_poll_t *handle,
                               const cloudabi_event_t *event) {
  assert((handle->__events & UV_WRITABLE) != 0 &&
         "Unnecessarily polled handle for reading");
  if (event->error == 0) {
    handle->__revents |= UV_WRITABLE;
    if ((event->fd_readwrite.flags & CLOUDABI_EVENT_FD_READWRITE_HANGUP) != 0)
      handle->__revents |= UV_DISCONNECT;
  } else {
    handle->__status = -event->error;
  }
}

static void __uv_poll_trigger(uv_poll_t *handle) {
  if (handle->__status != 0)
    handle->__cb(handle, handle->__status, 0);
  else if (handle->__revents != 0)
    handle->__cb(handle, 0, handle->__revents);
  handle->__status = 0;
  handle->__revents = 0;
}

static void __uv_process_proc_terminate(uv_process_t *handle,
                                        const cloudabi_event_t *event) {
  __uv_process_stop(handle);
  handle->__cb(handle, event->proc_terminate.exitcode,
               event->proc_terminate.signal);
}

static void __uv_stream_fd_read(uv_stream_t *handle,
                                const cloudabi_event_t *event) {
  // TODO(ed): Implement.
  assert(0 && "Not implemented");
}

static void __uv_stream_fd_write(uv_stream_t *handle,
                                 const cloudabi_event_t *event) {
  // This code assumes uv_buf_t and cloudabi_ciovec_t are identical.
  static_assert(offsetof(cloudabi_ciovec_t, buf) == offsetof(uv_buf_t, base),
                "Offset mismatch");
  static_assert(
      sizeof(((cloudabi_ciovec_t *)0)->buf) == sizeof(((uv_buf_t *)0)->base),
      "Size mismatch");
  static_assert(offsetof(cloudabi_ciovec_t, buf_len) == offsetof(uv_buf_t, len),
                "Offset mismatch");
  static_assert(
      sizeof(((cloudabi_ciovec_t *)0)->buf_len) == sizeof(((uv_buf_t *)0)->len),
      "Size mismatch");
  static_assert(sizeof(cloudabi_ciovec_t) == sizeof(uv_buf_t), "Size mismatch");

  while (!__uv_writes_empty(&handle->__write_queue)) {
    // Determine the number of iovecs to write. Be conservative by
    // limiting the number to _XOPEN_IOV_MAX, as more may cause the
    // write call to fail.
    uv_write_t *write = __uv_writes_first(&handle->__write_queue);
    const cloudabi_ciovec_t *iov =
        (const cloudabi_ciovec_t *)write->__bufs + write->__nbufs_done;
    size_t niov = write->__nbufs_total - write->__nbufs_done;
    if (niov > _XOPEN_IOV_MAX)
      niov = _XOPEN_IOV_MAX;
    assert(niov > 0 && "Request must have buffers");

    // Write data.
    size_t nwritten;
    cloudabi_errno_t error;
    if (write->send_handle != NULL) {
      // Initial write: send file descriptor across.
      assert(write->send_handle->__fd >= 0 && "Invalid stream");
      cloudabi_fd_t send_handle = write->send_handle->__fd;
      cloudabi_send_in_t si = {
          .si_data = iov,
          .si_data_len = niov,
          .si_fds = &send_handle,
          .si_fds_len = 1,
      };
      cloudabi_send_out_t so;
      error = cloudabi_sys_sock_send(handle->__fd, &si, &so);
      nwritten = so.so_datalen;
    } else {
      error = cloudabi_sys_fd_write(handle->__fd, iov, niov, &nwritten);
    }
    if (error != 0) {
      if (error == CLOUDABI_EAGAIN)
        break;

      // I/O failure. Discard all requests in the write queue.
      // TODO(ed): Implement!
      assert(0 && "Not implemented!");
    }
    write->send_handle = NULL;
    handle->write_queue_size -= nwritten;

    // Progress all iovecs that are written in full.
    while (write->__nbufs_done < write->__nbufs_total &&
           nwritten >= write->__bufs[write->__nbufs_done].len) {
      nwritten -= write->__bufs[write->__nbufs_done++].len;
    }

    if (write->__nbufs_done == write->__nbufs_total) {
      // Write finished. Complete it.
      assert(nwritten == 0 && "Wrote more bytes than in the request");
      free(write->__bufs);
      __uv_writes_remove(write);
      if (__uv_writes_empty(&handle->__write_queue)) {
        // TODO(ed): This is incorrect w.r.t. reads!
        __uv_handle_stop((uv_handle_t *)handle);
        __uv_writing_streams_remove(handle);
      }
      write->__cb(write, 0);
    } else {
      // Write not finished yet. Trim data that has been written.
      write->__bufs[write->__nbufs_done].base += nwritten;
      write->__bufs[write->__nbufs_done].len -= nwritten;
    }
  }

  assert(handle->write_queue_size == 0 && "Write queue size handling mismatch");

  // TODO(ed): Perform shutdown().
}

static void run_closing_handles(uv_loop_t *loop) {
  struct __uv_closing_handles_head closing_handles;
  __uv_closing_handles_move(&loop->__closing_handles, &closing_handles);
  while (!__uv_closing_handles_empty(&closing_handles)) {
    uv_handle_t *handle = __uv_closing_handles_first(&closing_handles);
    __uv_closing_handles_remove_first(&closing_handles);
    __uv_handles_remove(handle);

    // TODO(ed): Actually free resources!
    handle->__close_cb(handle);
  }
}

static bool allocate_subscription(uv_loop_t *loop, size_t *nsubscriptions,
                                  cloudabi_subscription_t **sub) {
  if (*nsubscriptions == loop->__subscriptions_capacity) {
    size_t capacity = 4;
    while (capacity <= *nsubscriptions)
      capacity *= 2;
    cloudabi_subscription_t *subscriptions =
        reallocarray(loop->__subscriptions_buffer,
                     sizeof(cloudabi_subscription_t), capacity);
    if (subscriptions == NULL)
      return false;
    loop->__subscriptions_buffer = subscriptions;
    loop->__subscriptions_capacity = capacity;
  }
  cloudabi_subscription_t *subscriptions = loop->__subscriptions_buffer;
  *sub = &subscriptions[(*nsubscriptions)++];
  return true;
}

static int do_poll(uv_loop_t *loop, int timeout) {
#define FOREACH(vartype, var, headtype, head)                                 \
  for (vartype *var = headtype##_empty(head) ? NULL : headtype##_first(head); \
       var != NULL;                                                           \
       var = var == headtype##_last(head) ? NULL : headtype##_next(var))
  // Create subscription for all of the active handles.
  size_t nsubscriptions = 0;
  FOREACH(uv_async_t, async, __uv_active_asyncs, &loop->__active_asyncs) {
    cloudabi_subscription_t *sub;
    if (!allocate_subscription(loop, &nsubscriptions, &sub))
      return UV_ENOMEM;
    *sub = (cloudabi_subscription_t){
        .userdata = (uintptr_t)async,
        .type = CLOUDABI_EVENTTYPE_FD_READ,
        .fd_readwrite.fd = async->__readfd,
        .fd_readwrite.flags = CLOUDABI_SUBSCRIPTION_FD_READWRITE_POLL,
    };
  }
  FOREACH(uv_poll_t, poll, __uv_reading_polls, &loop->__reading_polls) {
    assert(poll->__status == 0 && poll->__revents == 0 &&
           "Poll object still has pending events from a previous run");
    cloudabi_subscription_t *sub;
    if (!allocate_subscription(loop, &nsubscriptions, &sub))
      return UV_ENOMEM;
    *sub = (cloudabi_subscription_t){
        .userdata = (uintptr_t)poll,
        .type = CLOUDABI_EVENTTYPE_FD_READ,
        .fd_readwrite.fd = poll->__fd,
        .fd_readwrite.flags = CLOUDABI_SUBSCRIPTION_FD_READWRITE_POLL,
    };
  }
  FOREACH(uv_poll_t, poll, __uv_writing_polls, &loop->__writing_polls) {
    assert(poll->__status == 0 && poll->__revents == 0 &&
           "Poll object still has pending events from a previous run");
    cloudabi_subscription_t *sub;
    if (!allocate_subscription(loop, &nsubscriptions, &sub))
      return UV_ENOMEM;
    *sub = (cloudabi_subscription_t){
        .userdata = (uintptr_t)poll,
        .type = CLOUDABI_EVENTTYPE_FD_WRITE,
        .fd_readwrite.fd = poll->__fd,
        .fd_readwrite.flags = CLOUDABI_SUBSCRIPTION_FD_READWRITE_POLL,
    };
  }
  FOREACH(uv_process_t, process, __uv_active_processes,
          &loop->__active_processes) {
    cloudabi_subscription_t *sub;
    if (!allocate_subscription(loop, &nsubscriptions, &sub))
      return UV_ENOMEM;
    *sub = (cloudabi_subscription_t){
        .userdata = (uintptr_t)process,
        .type = CLOUDABI_EVENTTYPE_PROC_TERMINATE,
        .proc_terminate.fd = process->__fd,
    };
  }
  FOREACH(uv_stream_t, stream, __uv_reading_streams, &loop->__reading_streams) {
    cloudabi_subscription_t *sub;
    if (!allocate_subscription(loop, &nsubscriptions, &sub))
      return UV_ENOMEM;
    *sub = (cloudabi_subscription_t){
        .userdata = (uintptr_t)stream,
        .type = CLOUDABI_EVENTTYPE_FD_READ,
        .fd_readwrite.fd = stream->__fd,
        .fd_readwrite.flags = CLOUDABI_SUBSCRIPTION_FD_READWRITE_POLL,
    };
  }
  FOREACH(uv_stream_t, stream, __uv_writing_streams, &loop->__writing_streams) {
    cloudabi_subscription_t *sub;
    if (!allocate_subscription(loop, &nsubscriptions, &sub))
      return UV_ENOMEM;
    *sub = (cloudabi_subscription_t){
        .userdata = (uintptr_t)stream,
        .type = CLOUDABI_EVENTTYPE_FD_WRITE,
        .fd_readwrite.fd = stream->__fd,
        .fd_readwrite.flags = CLOUDABI_SUBSCRIPTION_FD_READWRITE_POLL,
    };
  }
#undef FOREACH

  // Add one extra subscription for the timeout, if any.
  if (timeout >= 0) {
    cloudabi_subscription_t *sub;
    if (!allocate_subscription(loop, &nsubscriptions, &sub))
      return UV_ENOMEM;
    static const uv_handle_t inactive_handle = {};
    *sub = (cloudabi_subscription_t){
        .userdata = (uintptr_t)&inactive_handle,
        .type = CLOUDABI_EVENTTYPE_CLOCK,
        .clock.clock_id = CLOUDABI_CLOCK_MONOTONIC,
        .clock.timeout = (uint64_t)timeout * 1000000,
        .clock.precision = 1000000,
    };
  }

  // Grow the events buffer to match the number of subscriptions.
  // Otherwise we wouldn't be able to extract all triggering events.
  if (loop->__events_capacity < nsubscriptions) {
    cloudabi_event_t *events =
        reallocarray(loop->__events_buffer, sizeof(cloudabi_event_t),
                     loop->__subscriptions_capacity);
    if (events == NULL)
      return UV_ENOMEM;
    loop->__events_buffer = events;
    loop->__events_capacity = loop->__subscriptions_capacity;
  }

  // Go to sleep.
  cloudabi_event_t *events = loop->__events_buffer;
  size_t ntriggered;
  cloudabi_errno_t error = cloudabi_sys_poll(
      loop->__subscriptions_buffer, events, nsubscriptions, &ntriggered);
  if (error != 0)
    return -error;

  // uv_poll_t is implemented by registering multiple events. First
  // iterate over all of the triggered events to recombine multiple
  // events for the same poll object.
  for (size_t i = 0; i < ntriggered; ++i) {
    const cloudabi_event_t *event = &events[i];
    uv_handle_t *handle = (uv_handle_t *)event->userdata;
    if (handle->type == UV_POLL) {
      switch (event->type) {
        case CLOUDABI_EVENTTYPE_FD_READ:
          __uv_poll_fd_read((uv_poll_t *)handle, event);
          break;
        case CLOUDABI_EVENTTYPE_FD_WRITE:
          __uv_poll_fd_write((uv_poll_t *)handle, event);
          break;
        default:
          assert(0 && "Unexpected event type");
      }
    }
  }

  // Iterate over all triggered events and invoke callbacks. Skip
  // handles that are not active. Those may have been stopped by earlier
  // callbacks in the same iteration.
  for (size_t i = 0; i < ntriggered; ++i) {
    const cloudabi_event_t *event = &events[i];
    uv_handle_t *handle = (uv_handle_t *)event->userdata;
    if (!uv_is_active(handle))
      continue;
    switch (handle->type) {
      case UV_ASYNC: {
        assert(event->type == CLOUDABI_EVENTTYPE_FD_READ &&
               "Unexpected event type");
        __uv_async_fd_read((uv_async_t *)handle, event);
        break;
      }
      case UV_POLL:
        __uv_poll_trigger((uv_poll_t *)handle);
        break;
      case UV_PROCESS: {
        assert(event->type == CLOUDABI_EVENTTYPE_PROC_TERMINATE &&
               "Unexpected event type");
        __uv_process_proc_terminate((uv_process_t *)handle, event);
        break;
      }
      case UV_NAMED_PIPE:
      case UV_TCP: {
        switch (event->type) {
          case CLOUDABI_EVENTTYPE_FD_READ:
            __uv_stream_fd_read((uv_stream_t *)handle, event);
            break;
          case CLOUDABI_EVENTTYPE_FD_WRITE:
            __uv_stream_fd_write((uv_stream_t *)handle, event);
            break;
          default:
            assert(0 && "Unexpected event type");
        }
        break;
      }
      default:
        assert(0 && "Unexpected handle type");
    }
  }
  return 0;
}

int uv_run(uv_loop_t *loop, uv_run_mode mode) {
  // "1. The loop concept of ‘now’ is updated."
  uv_update_time(loop);

  for (;;) {
    // "2. If the loop is alive an iteration is started, otherwise the
    // loop will exit immediately."
    if (uv_loop_alive(loop) == 0)
      break;

    // "3. Due timers are run."
    run_timers(loop);

    // "4. Pending callbacks are called."

    // "5. Idle handle callbacks are called."
    run_idles(loop);

    // "6. Prepare handle callbacks are called."
    run_prepares(loop);

    // "7. Poll timeout is calculated."
    int timeout = uv_backend_timeout(loop);

    // "8. The loop blocks for I/O."
    int error = do_poll(loop, timeout);
    if (error != 0)
      return error;

    // "9. Check handle callbacks are called."
    run_checks(loop);

    // "10. Close callbacks are called."
    run_closing_handles(loop);
  }

  loop->__stop = false;
  // TODO(ed): Return the right value.
  return 0;
}
