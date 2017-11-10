// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include "crypt_hash_sha256.h"

#define crypt_mcf_shaX crypt_mcf_sha256
#define BASE64_REMAP1 \
  { 20, 11, 2, 23, 14, 5, 26, 17, 8, 29, 30 }
#define BASE64_REMAP2 \
  { 10, 1, 22, 13, 4, 25, 16, 7, 28, 19, 31 }
#define BASE64_REMAP3 \
  { 0, 21, 12, 3, 24, 15, 6, 27, 18, 9 }
#define SHAX_CTX SHA256_CTX
#define SHAX_DIGEST_LENGTH SHA256_DIGEST_LENGTH
#define SHAX_Final SHA256_Final
#define SHAX_Init SHA256_Init
#define SHAX_Update SHA256_Update
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
