// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared) {
  switch (pshared) {
    case PTHREAD_PROCESS_PRIVATE:
    case PTHREAD_PROCESS_SHARED:
      attr->__pshared = pshared;
      return 0;
    default:
      return EINVAL;
  }
}

__strong_reference(pthread_rwlockattr_setpshared, pthread_mutexattr_setpshared);
