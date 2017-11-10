// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <uv.h>

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
  // This code assumes uv_buf_t and cloudabi_iovec_t are identical.
  static_assert(offsetof(cloudabi_iovec_t, buf) == offsetof(uv_buf_t, base),
                "Offset mismatch");
  static_assert(
      sizeof(((cloudabi_iovec_t *)0)->buf) == sizeof(((uv_buf_t *)0)->base),
      "Size mismatch");
  static_assert(offsetof(cloudabi_iovec_t, buf_len) == offsetof(uv_buf_t, len),
                "Offset mismatch");
  static_assert(
      sizeof(((cloudabi_iovec_t *)0)->buf_len) == sizeof(((uv_buf_t *)0)->len),
      "Size mismatch");
  static_assert(sizeof(cloudabi_iovec_t) == sizeof(uv_buf_t), "Size mismatch");

  // Place an upper bound on the number of reads we're willing to do to
  // prevent starvation of other handles. Take into account that the
  // stream may have been stopped for reading in the meantime.
  for (int read_iteration = 0; handle->__read_cb != NULL && read_iteration < 20;
       ++read_iteration) {
    uv_buf_t buf = uv_buf_init(NULL, 0);
    handle->__alloc_cb((uv_handle_t *)handle, 65536, &buf);
    if (buf.base == NULL || buf.len == 0) {
      // Allocator ran out of memory.
      handle->__read_cb(handle, UV_ENOBUFS, &buf);
      break;
    }

    cloudabi_errno_t error;
    size_t nread;
    if (__uv_stream_has_ipc(handle)) {
      // Stream is capable of receiving file descriptors. Allow
      // receiving up to 64 file descriptors in one go, which is the
      // same limit that the official implementation uses.
      cloudabi_fd_t received_fds[64];
      cloudabi_recv_in_t ri = {
          .ri_data = (cloudabi_iovec_t *)&buf,
          .ri_data_len = 1,
          .ri_fds = received_fds,
          .ri_fds_len = __arraycount(received_fds),
      };
      cloudabi_recv_out_t ro;
      error = cloudabi_sys_sock_recv(handle->__fd, &ri, &ro);
      if (error == 0) {
        nread = ro.ro_datalen;

        // Enqueue received file descriptors.
        for (size_t i = 0; i < ro.ro_fdslen; ++i) {
          if (!__uv_pending_fds_insert_last(&handle->__pending_fds,
                                            received_fds[i])) {
            do {
              cloudabi_sys_fd_close(received_fds[i]);
            } while (++i < ro.ro_fdslen);
            error = CLOUDABI_ENOMEM;
            break;
          }
        }
      }
    } else {
      // Stream without IPC. Don't assume it supports socket operations.
      error = cloudabi_sys_fd_read(handle->__fd, (cloudabi_iovec_t *)&buf, 1,
                                   &nread);
    }

    if (error == CLOUDABI_EAGAIN) {
      // Spurious wakeup.
      handle->__read_cb(handle, 0, &buf);
      break;
    } else if (error != 0) {
      // Read error.
      uv_read_cb cb = handle->__read_cb;
      handle->__read_cb = NULL;
      __uv_stream_stop_reading(handle);
      cb(handle, -error, &buf);
      break;
    } else if (nread > 0) {
      // Successfully read data.
      bool partial_read = nread < buf.len;
      handle->__read_cb(handle, nread, &buf);
      if (partial_read)
        break;
    } else {
      // End-of-file.
      uv_read_cb cb = handle->__read_cb;
      handle->__read_cb = NULL;
      __uv_stream_stop_reading(handle);
      cb(handle, UV_EOF, &buf);
      break;
    }
  }
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

  // Process write requests.
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

    if (error == 0) {
      // Managed to write data.
      write->send_handle = NULL;
      handle->write_queue_size -= nwritten;

      // Progress all iovecs that are written in full.
      while (write->__nbufs_done < write->__nbufs_total &&
             nwritten >= write->__bufs[write->__nbufs_done].len) {
        nwritten -= write->__bufs[write->__nbufs_done++].len;
      }

      if (write->__nbufs_done < write->__nbufs_total) {
        // Write not finished yet. Trim data that has been written.
        write->__bufs[write->__nbufs_done].base += nwritten;
        write->__bufs[write->__nbufs_done].len -= nwritten;
        continue;
      }

      // Write finished. Complete it.
      assert(nwritten == 0 && "Wrote more bytes than in the request");
    } else if (error == CLOUDABI_EAGAIN) {
      // File descriptor is now blocking again. Stop writing.
      break;
    } else {
      // Write failed. Discard the request.
      for (unsigned int i = write->__nbufs_done; i < write->__nbufs_total; ++i)
        handle->write_queue_size -= write->__bufs[i].len;
    }

    free(write->__bufs);
    __uv_writes_remove(write);
    __uv_stream_stop_writing(handle);
    write->__cb(write, -error);
  }
  assert(handle->write_queue_size == 0 && "Write queue size handling mismatch");

  // Process shutdown requests.
  if (!__uv_shutdowns_empty(&handle->__shutdown_queue)) {
    cloudabi_errno_t error =
        cloudabi_sys_sock_shutdown(handle->__fd, CLOUDABI_SHUT_WR);
    do {
      uv_shutdown_t *shutdown = __uv_shutdowns_first(&handle->__shutdown_queue);
      __uv_shutdowns_remove(shutdown);
      __uv_stream_stop_writing(handle);
      shutdown->__cb(shutdown, -error);
    } while (!__uv_shutdowns_empty(&handle->__shutdown_queue));
  }
}

static void run_closing_handles(uv_loop_t *loop) {
  struct __uv_closing_handles_head closing_handles;
  __uv_closing_handles_move(&loop->__closing_handles, &closing_handles);
  while (!__uv_closing_handles_empty(&closing_handles)) {
    uv_handle_t *handle = __uv_closing_handles_first(&closing_handles);
    __uv_closing_handles_remove_first(&closing_handles);
    __uv_handles_remove(handle);

    if (handle->type == UV_NAMED_PIPE || handle->type == UV_TCP ||
        handle->type == UV_TTY) {
      // Cancel all of the pending shutdowns and writes.
      uv_stream_t *stream = (uv_stream_t *)handle;
      while (!__uv_shutdowns_empty(&stream->__shutdown_queue)) {
        uv_shutdown_t *req = __uv_shutdowns_first(&stream->__shutdown_queue);
        __uv_shutdowns_remove(req);
        req->__cb(req, UV_ECANCELED);
      }
      while (!__uv_writes_empty(&stream->__write_queue)) {
        uv_write_t *req = __uv_writes_first(&stream->__write_queue);
        free(req->__bufs);
        __uv_writes_remove(req);
        req->__cb(req, UV_ECANCELED);
      }
      __uv_pending_fds_destroy(&stream->__pending_fds);
    }

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

static bool allocate_event(uv_loop_t *loop, size_t *nevents,
                           cloudabi_event_t **ev) {
  if (*nevents == loop->__events_capacity) {
    size_t capacity = 4;
    while (capacity <= *nevents)
      capacity *= 2;
    cloudabi_event_t *events =
        reallocarray(loop->__events_buffer, sizeof(cloudabi_event_t), capacity);
    if (events == NULL)
      return false;
    loop->__events_buffer = events;
    loop->__events_capacity = capacity;
  }
  cloudabi_event_t *events = loop->__events_buffer;
  *ev = &events[(*nevents)++];
  return true;
}

static int do_poll(uv_loop_t *loop, int timeout) {
#define FOREACH(vartype, var, headtype, head)                                 \
  for (vartype *var = headtype##_empty(head) ? NULL : headtype##_first(head); \
       var != NULL;                                                           \
       var = var == headtype##_last(head) ? NULL : headtype##_next(var))
  // Create subscription for all of the active handles.
  size_t nsubscriptions = 0;
  size_t nevents = 0;
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
    if (process->__fd >= 0) {
      // Process descriptor is still open. Ask the kernel whether the
      // process is still running.
      cloudabi_subscription_t *sub;
      if (!allocate_subscription(loop, &nsubscriptions, &sub))
        return UV_ENOMEM;
      *sub = (cloudabi_subscription_t){
          .userdata = (uintptr_t)process,
          .type = CLOUDABI_EVENTTYPE_PROC_TERMINATE,
          .proc_terminate.fd = process->__fd,
      };
    } else {
      // Process has been killed using uv_process_kill(). We can no
      // longer poll on it, so generate a fictive SIGKILL event.
      cloudabi_event_t *ev;
      if (!allocate_event(loop, &nevents, &ev))
        return UV_ENOMEM;
      *ev = (cloudabi_event_t){
          .userdata = (uintptr_t)process,
          .type = CLOUDABI_EVENTTYPE_PROC_TERMINATE,
          .proc_terminate.signal = SIGKILL,
      };
    }
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

  // If the loop above has already yielded events, then we should do a
  // non-blocking poll to extend the results.
  if (nevents > 0)
    timeout = 0;

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
  if (loop->__events_capacity < nevents + nsubscriptions) {
    size_t capacity = 4;
    while (capacity < nevents + nsubscriptions)
      capacity *= 2;
    cloudabi_event_t *events =
        reallocarray(loop->__events_buffer, sizeof(cloudabi_event_t), capacity);
    if (events == NULL)
      return UV_ENOMEM;
    loop->__events_buffer = events;
    loop->__events_capacity = capacity;
  }

  // Go to sleep.
  cloudabi_event_t *events = loop->__events_buffer;
  {
    size_t ntriggered;
    cloudabi_errno_t error =
        cloudabi_sys_poll(loop->__subscriptions_buffer, events + nevents,
                          nsubscriptions, &ntriggered);
    if (error != 0)
      return -error;
    nevents += ntriggered;
  }

  // uv_poll_t is implemented by registering multiple events. First
  // iterate over all of the triggered events to recombine multiple
  // events for the same poll object.
  for (size_t i = 0; i < nevents; ++i) {
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
  for (size_t i = 0; i < nevents; ++i) {
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
      case UV_TCP:
      case UV_TTY: {
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
  bool first_iteration = true;
  do {
    // "1. The loop concept of ‘now’ is updated."
    uv_update_time(loop);

    // "2. If the loop is alive an iteration is started, otherwise the
    // loop will exit immediately."
    if (uv_loop_alive(loop) == 0 || loop->__stop)
      break;

    // "3. Due timers are run."
    run_timers(loop);

    // "11. Special case in case the loop was run with UV_RUN_ONCE, as
    // it implies forward progress."
    if (!first_iteration && mode == UV_RUN_ONCE)
      break;

    // "4. Pending callbacks are called."
    // This implementation does not have any pending callbacks.

    // "5. Idle handle callbacks are called."
    run_idles(loop);

    // "6. Prepare handle callbacks are called."
    run_prepares(loop);

    // "7. Poll timeout is calculated."
    int timeout = mode == UV_RUN_NOWAIT ? 0 : uv_backend_timeout(loop);

    // "8. The loop blocks for I/O."
    int error = do_poll(loop, timeout);
    if (error != 0)
      return error;

    // "9. Check handle callbacks are called."
    run_checks(loop);

    // "10. Close callbacks are called."
    run_closing_handles(loop);

    first_iteration = false;
  } while (mode != UV_RUN_NOWAIT);

  loop->__stop = false;
  return uv_loop_alive(loop);
}
