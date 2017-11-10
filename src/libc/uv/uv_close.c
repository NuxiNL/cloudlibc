// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <limits.h>
#include <uv.h>

static void __uv_async_stop(uv_async_t *handle) {
  assert(uv_is_active((uv_handle_t *)handle) &&
         "Asyncs must always be active, except when closed");

  cloudabi_sys_fd_close(handle->__readfd);
  cloudabi_sys_fd_close(handle->__writefd);
  __uv_handle_stop((uv_handle_t *)handle);
  __uv_active_asyncs_remove(handle);
}

static void __uv_stream_stop(uv_stream_t *handle) {
  // Stop the stream for reading.
  uv_read_stop(handle);

  // Stop the stream for writing.
  if (!__uv_shutdowns_empty(&handle->__shutdown_queue) ||
      !__uv_writes_empty(&handle->__write_queue)) {
    __uv_handle_stop((uv_handle_t *)handle);
    __uv_writing_streams_remove(handle);
  }

  // Close the file descriptor and set it to -2, as opposed to -1. This
  // allows us to distinguish between the initial state (__fd == -1) and
  // valid (__fd >= 0).
  cloudabi_sys_fd_close(handle->__fd);
  handle->__fd = -2;

  // Close pending incoming file descriptors.
  while (!__uv_pending_fds_empty(&handle->__pending_fds)) {
    cloudabi_sys_fd_close(__uv_pending_fds_first(&handle->__pending_fds));
    __uv_pending_fds_remove_first(&handle->__pending_fds);
  }
}

void uv_close(uv_handle_t *handle, uv_close_cb close_cb) {
  assert(!uv_is_closing(handle) && "Attempted to double close handle");

  // Require the handle to be deactivated.
  switch (handle->type) {
    case UV_ASYNC:
      __uv_async_stop((uv_async_t *)handle);
      break;
    case UV_CHECK:
      uv_check_stop((uv_check_t *)handle);
      break;
    case UV_IDLE:
      uv_idle_stop((uv_idle_t *)handle);
      break;
    case UV_NAMED_PIPE:
    case UV_TCP:
    case UV_TTY:
      __uv_stream_stop((uv_stream_t *)handle);
      break;
    case UV_POLL:
      uv_poll_stop((uv_poll_t *)handle);
      break;
    case UV_PREPARE:
      uv_prepare_stop((uv_prepare_t *)handle);
      break;
    case UV_PROCESS:
      __uv_process_stop((uv_process_t *)handle);
      break;
    case UV_TIMER:
      uv_timer_stop((uv_timer_t *)handle);
      break;
    default:
      assert(0 && "Attempted to close handle of an unsupported type");
  }
  assert(!uv_is_active(handle) && "Attempted to close an active handle");

  // Switch handle to the closing state.
  handle->__state |= UV_HANDLE_CLOSING;
  handle->__close_cb = close_cb;
  __uv_closing_handles_insert_first(&handle->loop->__closing_handles, handle);
}
