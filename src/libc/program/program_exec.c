// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <cloudabi_syscalls.h>
#include <program.h>

int program_exec(int fd, const argdata_t *ad) {
  // Compute size needed for serializing the argument data.
  size_t datalen;
  size_t fdslen;
  argdata_get_buffer_length(ad, &datalen, &fdslen);

  // Allocate space for the serialized the argument data.
  size_t mappinglen = fdslen * sizeof(int) + datalen;
  void *mapping;
  cloudabi_errno_t error = cloudabi_sys_mem_map(
      NULL, mappinglen, CLOUDABI_PROT_READ | CLOUDABI_PROT_WRITE,
      CLOUDABI_MAP_ANON | CLOUDABI_MAP_PRIVATE, CLOUDABI_MAP_ANON_FD, 0,
      &mapping);
  if (error != 0)
    return error;

  // Serialize the argument data.
  int *fds = mapping;
  char *data = (char *)&fds[fdslen];
  fdslen = argdata_get_buffer(ad, data, fds);

  // Execute the new process, providing it the serialized argument data
  // and a list of file descriptors.
  error =
      cloudabi_sys_proc_exec(fd, data, datalen, (cloudabi_fd_t *)fds, fdslen);
  cloudabi_sys_mem_unmap(mapping, mappinglen);
  return error;
}
