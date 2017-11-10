// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/errno.h>
#include <common/uv.h>

#include <argdata.h>
#include <cloudabi_syscalls.h>
#include <errno.h>
#include <program.h>
#include <stdlib.h>
#include <uv.h>

int program_spawn(uv_loop_t *loop, uv_process_t *handle, int fd,
                  const argdata_t *ad, uv_exit_cb cb) {
  // Serialize the argument data.
  size_t datalen;
  size_t fdslen;
  argdata_serialized_length(ad, &datalen, &fdslen);
  int *fds = malloc(fdslen * sizeof(int) + datalen);
  if (fds == NULL)
    return ENOMEM;
  char *data = (char *)&fds[fdslen];
  fdslen = argdata_serialize(ad, data, fds);

  // Create a pipe to send proc_exec()'s error code back to the parent.
  cloudabi_fd_t error_read_fd, error_write_fd;
  {
    cloudabi_errno_t error = cloudabi_sys_fd_create2(
        CLOUDABI_FILETYPE_SOCKET_STREAM, &error_read_fd, &error_write_fd);
    if (error != 0) {
      free(fds);
      return error;
    }
  }

  // Fork.
  cloudabi_fd_t process_fd;
  {
    cloudabi_tid_t thread_id;
    cloudabi_errno_t error = cloudabi_sys_proc_fork(&process_fd, &thread_id);
    if (error != 0) {
      free(fds);
      cloudabi_sys_fd_close(error_read_fd);
      cloudabi_sys_fd_close(error_write_fd);
      return error;
    }
  }

  // In the child process, attempt to execute. If that fails, write the
  // error code into the pipe.
  if (process_fd == CLOUDABI_PROCESS_CHILD) {
    cloudabi_errno_t error = errno_fixup_executable(
        fd, cloudabi_sys_proc_exec(fd, data, datalen, (cloudabi_fd_t *)fds,
                                   fdslen));
    cloudabi_ciovec_t error_buf = {.buf = &error, .buf_len = sizeof(error)};
    size_t nwritten;
    cloudabi_sys_fd_write(error_write_fd, &error_buf, 1, &nwritten);
    cloudabi_sys_proc_exit(127);
  }

  free(fds);
  cloudabi_sys_fd_close(error_write_fd);

  // Attempt to read any error code from the pipe.
  {
    cloudabi_errno_t exec_error;
    cloudabi_iovec_t buf = {.buf = &exec_error, .buf_len = sizeof(exec_error)};
    size_t nread;
    cloudabi_errno_t error =
        cloudabi_sys_fd_read(error_read_fd, &buf, 1, &nread);
    cloudabi_sys_fd_close(error_read_fd);
    if (error == 0 && nread == sizeof(exec_error) && exec_error != 0) {
      cloudabi_sys_fd_close(process_fd);
      return exec_error;
    }
  }

  // Program spawned successfully.
  __uv_handle_init(loop, (uv_handle_t *)handle, UV_PROCESS);
  __uv_handle_start((uv_handle_t *)handle);
  __uv_active_processes_insert_last(&loop->__active_processes, handle);
  handle->__cb = cb;
  handle->__fd = process_fd;
  return 0;
}
