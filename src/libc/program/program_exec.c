// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/errno.h>

#include <argdata.h>
#include <cloudabi_syscalls.h>
#include <program.h>

int program_exec(int fd, const argdata_t *ad) {
  // Compute size needed for serializing the argument data.
  size_t datalen;
  size_t fdslen;
  argdata_serialized_length(ad, &datalen, &fdslen);

  cloudabi_errno_t error;
  size_t mappinglen = fdslen * sizeof(int) + datalen;
  if (mappinglen == 0) {
    // Corner case: don't attempt to allocate zero bytes.
    error = cloudabi_sys_proc_exec(fd, NULL, 0, NULL, 0);
  } else {
    // Allocate space for the serialized the argument data.
    void *mapping;
    error = cloudabi_sys_mem_map(NULL, mappinglen,
                                 CLOUDABI_PROT_READ | CLOUDABI_PROT_WRITE,
                                 CLOUDABI_MAP_ANON | CLOUDABI_MAP_PRIVATE,
                                 CLOUDABI_MAP_ANON_FD, 0, &mapping);
    if (error != 0)
      return error;

    // Serialize the argument data.
    int *fds = mapping;
    char *data = (char *)&fds[fdslen];
    fdslen = argdata_serialize(ad, data, fds);

    // Execute the new process, providing it the serialized argument data
    // and a list of file descriptors.
    error = errno_fixup_executable(
        fd, cloudabi_sys_proc_exec(fd, data, datalen, (cloudabi_fd_t *)fds,
                                   fdslen));
    cloudabi_sys_mem_unmap(mapping, mappinglen);
  }
  return error;
}
