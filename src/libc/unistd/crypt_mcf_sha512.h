// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include "crypt_hash_sha512.h"

#define crypt_mcf_shaX crypt_mcf_sha512
#define BASE64_REMAP1                                                        \
  {                                                                          \
    42, 1, 23, 45, 4, 26, 48, 7, 29, 51, 10, 32, 54, 13, 35, 57, 16, 38, 60, \
        19, 41, 63                                                           \
  }
#define BASE64_REMAP2                                                        \
  {                                                                          \
    21, 43, 2, 24, 46, 5, 27, 49, 8, 30, 52, 11, 33, 55, 14, 36, 58, 17, 39, \
        61, 20                                                               \
  }
#define BASE64_REMAP3                                                       \
  {                                                                         \
    0, 22, 44, 3, 25, 47, 6, 28, 50, 9, 31, 53, 12, 34, 56, 15, 37, 59, 18, \
        40, 62                                                              \
  }
#define SHAX_CTX SHA512_CTX
#define SHAX_DIGEST_LENGTH SHA512_DIGEST_LENGTH
#define SHAX_Final SHA512_Final
#define SHAX_Init SHA512_Init
#define SHAX_Update SHA512_Update
#include "crypt_mcf_shaX.h"
#undef crypt_mcf_shaX
#undef BASE64_REMAP1
#undef BASE64_REMAP2
#undef BASE64_REMAP3
#undef SHAX_CTX
#undef SHAX_DIGEST_LENGTH
#undef SHAX_Final
#undef SHAX_Init
#undef SHAX_Update
