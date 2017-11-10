// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared) {
  switch (pshared) {
    case PTHREAD_PROCESS_PRIVATE:
    case PTHREAD_PROCESS_SHARED:
      attr->__pshared = pshared;
      return 0;
    default:
      return EINVAL;
  }
}
