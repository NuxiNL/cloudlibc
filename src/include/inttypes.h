// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <inttypes.h> - fixed size integer types
//
// Extensions:
// - strto*max_l():
//   strto*max() always uses the C locale.

#ifndef _INTTYPES_H_
#define _INTTYPES_H_

#include <stdint.h>

#ifndef _WCHAR_T_DECLARED
typedef __wchar_t wchar_t;
#define _WCHAR_T_DECLARED
#endif

typedef struct {
  intmax_t quot;
  intmax_t rem;
} imaxdiv_t;

// printf() modifiers.

#define PRId8 __INT8_FMTd__
#define PRIi8 __INT8_FMTi__
#define PRIo8 __UINT8_FMTo__
#define PRIu8 __UINT8_FMTu__
#define PRIx8 __UINT8_FMTx__
#define PRIX8 __UINT8_FMTX__
#define PRId16 __INT16_FMTd__
#define PRIi16 __INT16_FMTi__
#define PRIo16 __UINT16_FMTo__
#define PRIu16 __UINT16_FMTu__
#define PRIx16 __UINT16_FMTx__
#define PRIX16 __UINT16_FMTX__
#ifdef __INT24_TYPE__
#define PRId24 __INT24_FMTd__
#define PRIi24 __INT24_FMTi__
#define PRIo24 __UINT24_FMTo__
#define PRIu24 __UINT24_FMTu__
#define PRIx24 __UINT24_FMTx__
#define PRIX24 __UINT24_FMTX__
#endif
#define PRId32 __INT32_FMTd__
#define PRIi32 __INT32_FMTi__
#define PRIo32 __UINT32_FMTo__
#define PRIu32 __UINT32_FMTu__
#define PRIx32 __UINT32_FMTx__
#define PRIX32 __UINT32_FMTX__
#ifdef __INT40_TYPE__
#define PRId40 __INT40_FMTd__
#define PRIi40 __INT40_FMTi__
#define PRIo40 __UINT40_FMTo__
#define PRIu40 __UINT40_FMTu__
#define PRIx40 __UINT40_FMTx__
#define PRIX40 __UINT40_FMTX__
#endif
#ifdef __INT48_TYPE__
#define PRId48 __INT48_FMTd__
#define PRIi48 __INT48_FMTi__
#define PRIo48 __UINT48_FMTo__
#define PRIu48 __UINT48_FMTu__
#define PRIx48 __UINT48_FMTx__
#define PRIX48 __UINT48_FMTX__
#endif
#ifdef __INT56_TYPE__
#define PRId56 __INT56_FMTd__
#define PRIi56 __INT56_FMTi__
#define PRIo56 __UINT56_FMTo__
#define PRIu56 __UINT56_FMTu__
#define PRIx56 __UINT56_FMTx__
#define PRIX56 __UINT56_FMTX__
#endif
#define PRId64 __INT64_FMTd__
#define PRIi64 __INT64_FMTi__
#define PRIo64 __UINT64_FMTo__
#define PRIu64 __UINT64_FMTu__
#define PRIx64 __UINT64_FMTx__
#define PRIX64 __UINT64_FMTX__

#define PRIdLEAST8 PRId8
#define PRIiLEAST8 PRIi8
#define PRIoLEAST8 PRIo8
#define PRIuLEAST8 PRIu8
#define PRIxLEAST8 PRIx8
#define PRIXLEAST8 PRIX8
#define PRIdLEAST16 PRId16
#define PRIiLEAST16 PRIi16
#define PRIoLEAST16 PRIo16
#define PRIuLEAST16 PRIu16
#define PRIxLEAST16 PRIx16
#define PRIXLEAST16 PRIX16
#ifdef PRId24
#define PRIdLEAST24 PRId24
#define PRIiLEAST24 PRIi24
#define PRIoLEAST24 PRIo24
#define PRIuLEAST24 PRIu24
#define PRIxLEAST24 PRIx24
#define PRIXLEAST24 PRIX24
#else
#define PRIdLEAST24 PRIdLEAST32
#define PRIiLEAST24 PRIiLEAST32
#define PRIoLEAST24 PRIoLEAST32
#define PRIuLEAST24 PRIuLEAST32
#define PRIxLEAST24 PRIxLEAST32
#define PRIXLEAST24 PRIXLEAST32
#endif
#define PRIdLEAST32 PRId32
#define PRIiLEAST32 PRIi32
#define PRIoLEAST32 PRIo32
#define PRIuLEAST32 PRIu32
#define PRIxLEAST32 PRIx32
#define PRIXLEAST32 PRIX32
#ifdef PRId40
#define PRIdLEAST40 PRId40
#define PRIiLEAST40 PRIi40
#define PRIoLEAST40 PRIo40
#define PRIuLEAST40 PRIu40
#define PRIxLEAST40 PRIx40
#define PRIXLEAST40 PRIX40
#else
#define PRIdLEAST40 PRIdLEAST48
#define PRIiLEAST40 PRIiLEAST48
#define PRIoLEAST40 PRIoLEAST48
#define PRIuLEAST40 PRIuLEAST48
#define PRIxLEAST40 PRIxLEAST48
#define PRIXLEAST40 PRIXLEAST48
#endif
#ifdef PRId48
#define PRIdLEAST48 PRId48
#define PRIiLEAST48 PRIi48
#define PRIoLEAST48 PRIo48
#define PRIuLEAST48 PRIu48
#define PRIxLEAST48 PRIx48
#define PRIXLEAST48 PRIX48
#else
#define PRIdLEAST48 PRIdLEAST56
#define PRIiLEAST48 PRIiLEAST56
#define PRIoLEAST48 PRIoLEAST56
#define PRIuLEAST48 PRIuLEAST56
#define PRIxLEAST48 PRIxLEAST56
#define PRIXLEAST48 PRIXLEAST56
#endif
#ifdef PRId56
#define PRIdLEAST56 PRId56
#define PRIiLEAST56 PRIi56
#define PRIoLEAST56 PRIo56
#define PRIuLEAST56 PRIu56
#define PRIxLEAST56 PRIx56
#define PRIXLEAST56 PRIX56
#else
#define PRIdLEAST56 PRIdLEAST64
#define PRIiLEAST56 PRIiLEAST64
#define PRIoLEAST56 PRIoLEAST64
#define PRIuLEAST56 PRIuLEAST64
#define PRIxLEAST56 PRIxLEAST64
#define PRIXLEAST56 PRIXLEAST64
#endif
#define PRIdLEAST64 PRId64
#define PRIiLEAST64 PRIi64
#define PRIoLEAST64 PRIo64
#define PRIuLEAST64 PRIu64
#define PRIxLEAST64 PRIx64
#define PRIXLEAST64 PRIX64

#define PRIdFAST8 PRIdLEAST8
#define PRIiFAST8 PRIiLEAST8
#define PRIoFAST8 PRIoLEAST8
#define PRIuFAST8 PRIuLEAST8
#define PRIxFAST8 PRIxLEAST8
#define PRIXFAST8 PRIXLEAST8
#define PRIdFAST16 PRIdLEAST16
#define PRIiFAST16 PRIiLEAST16
#define PRIoFAST16 PRIoLEAST16
#define PRIuFAST16 PRIuLEAST16
#define PRIxFAST16 PRIxLEAST16
#define PRIXFAST16 PRIXLEAST16
#define PRIdFAST24 PRIdLEAST24
#define PRIiFAST24 PRIiLEAST24
#define PRIoFAST24 PRIoLEAST24
#define PRIuFAST24 PRIuLEAST24
#define PRIxFAST24 PRIxLEAST24
#define PRIXFAST24 PRIXLEAST24
#define PRIdFAST32 PRIdLEAST32
#define PRIiFAST32 PRIiLEAST32
#define PRIoFAST32 PRIoLEAST32
#define PRIuFAST32 PRIuLEAST32
#define PRIxFAST32 PRIxLEAST32
#define PRIXFAST32 PRIXLEAST32
#define PRIdFAST40 PRIdLEAST40
#define PRIiFAST40 PRIiLEAST40
#define PRIoFAST40 PRIoLEAST40
#define PRIuFAST40 PRIuLEAST40
#define PRIxFAST40 PRIxLEAST40
#define PRIXFAST40 PRIXLEAST40
#define PRIdFAST48 PRIdLEAST48
#define PRIiFAST48 PRIiLEAST48
#define PRIoFAST48 PRIoLEAST48
#define PRIuFAST48 PRIuLEAST48
#define PRIxFAST48 PRIxLEAST48
#define PRIXFAST48 PRIXLEAST48
#define PRIdFAST56 PRIdLEAST56
#define PRIiFAST56 PRIiLEAST56
#define PRIoFAST56 PRIoLEAST56
#define PRIuFAST56 PRIuLEAST56
#define PRIxFAST56 PRIxLEAST56
#define PRIXFAST56 PRIXLEAST56
#define PRIdFAST64 PRIdLEAST64
#define PRIiFAST64 PRIiLEAST64
#define PRIoFAST64 PRIoLEAST64
#define PRIuFAST64 PRIuLEAST64
#define PRIxFAST64 PRIxLEAST64
#define PRIXFAST64 PRIXLEAST64

#define PRIdMAX "jd"
#define PRIiMAX "ji"
#define PRIoMAX "jo"
#define PRIuMAX "ju"
#define PRIxMAX "jx"
#define PRIXMAX "jX"

#define PRIdPTR __INTPTR_FMTd__
#define PRIiPTR __INTPTR_FMTi__
#define PRIoPTR __UINTPTR_FMTo__
#define PRIuPTR __UINTPTR_FMTu__
#define PRIxPTR __UINTPTR_FMTx__
#define PRIXPTR __UINTPTR_FMTX__

// scanf() modifiers.

#define SCNd8 __INT8_FMTd__
#define SCNi8 __INT8_FMTi__
#define SCNo8 __UINT8_FMTo__
#define SCNu8 __UINT8_FMTu__
#define SCNx8 __UINT8_FMTx__
#define SCNd16 __INT16_FMTd__
#define SCNi16 __INT16_FMTi__
#define SCNo16 __UINT16_FMTo__
#define SCNu16 __UINT16_FMTu__
#define SCNx16 __UINT16_FMTx__
#ifdef __INT24_TYPE__
#define SCNd24 __INT24_FMTd__
#define SCNi24 __INT24_FMTi__
#define SCNo24 __UINT24_FMTo__
#define SCNu24 __UINT24_FMTu__
#define SCNx24 __UINT24_FMTx__
#endif
#define SCNd32 __INT32_FMTd__
#define SCNi32 __INT32_FMTi__
#define SCNo32 __UINT32_FMTo__
#define SCNu32 __UINT32_FMTu__
#define SCNx32 __UINT32_FMTx__
#ifdef __INT40_TYPE__
#define SCNd40 __INT40_FMTd__
#define SCNi40 __INT40_FMTi__
#define SCNo40 __UINT40_FMTo__
#define SCNu40 __UINT40_FMTu__
#define SCNx40 __UINT40_FMTx__
#endif
#ifdef __INT48_TYPE__
#define SCNd48 __INT48_FMTd__
#define SCNi48 __INT48_FMTi__
#define SCNo48 __UINT48_FMTo__
#define SCNu48 __UINT48_FMTu__
#define SCNx48 __UINT48_FMTx__
#endif
#ifdef __INT56_TYPE__
#define SCNd56 __INT56_FMTd__
#define SCNi56 __INT56_FMTi__
#define SCNo56 __UINT56_FMTo__
#define SCNu56 __UINT56_FMTu__
#define SCNx56 __UINT56_FMTx__
#endif
#define SCNd64 __INT64_FMTd__
#define SCNi64 __INT64_FMTi__
#define SCNo64 __UINT64_FMTo__
#define SCNu64 __UINT64_FMTu__
#define SCNx64 __UINT64_FMTx__

#define SCNdLEAST8 SCNd8
#define SCNiLEAST8 SCNi8
#define SCNoLEAST8 SCNo8
#define SCNuLEAST8 SCNu8
#define SCNxLEAST8 SCNx8
#define SCNdLEAST16 SCNd16
#define SCNiLEAST16 SCNi16
#define SCNoLEAST16 SCNo16
#define SCNuLEAST16 SCNu16
#define SCNxLEAST16 SCNx16
#ifdef SCNd24
#define SCNdLEAST24 SCNd24
#define SCNiLEAST24 SCNi24
#define SCNoLEAST24 SCNo24
#define SCNuLEAST24 SCNu24
#define SCNxLEAST24 SCNx24
#else
#define SCNdLEAST24 SCNdLEAST32
#define SCNiLEAST24 SCNiLEAST32
#define SCNoLEAST24 SCNoLEAST32
#define SCNuLEAST24 SCNuLEAST32
#define SCNxLEAST24 SCNxLEAST32
#endif
#define SCNdLEAST32 SCNd32
#define SCNiLEAST32 SCNi32
#define SCNoLEAST32 SCNo32
#define SCNuLEAST32 SCNu32
#define SCNxLEAST32 SCNx32
#ifdef SCNd40
#define SCNdLEAST40 SCNd40
#define SCNiLEAST40 SCNi40
#define SCNoLEAST40 SCNo40
#define SCNuLEAST40 SCNu40
#define SCNxLEAST40 SCNx40
#else
#define SCNdLEAST40 SCNdLEAST48
#define SCNiLEAST40 SCNiLEAST48
#define SCNoLEAST40 SCNoLEAST48
#define SCNuLEAST40 SCNuLEAST48
#define SCNxLEAST40 SCNxLEAST48
#endif
#ifdef SCNd48
#define SCNdLEAST48 SCNd48
#define SCNiLEAST48 SCNi48
#define SCNoLEAST48 SCNo48
#define SCNuLEAST48 SCNu48
#define SCNxLEAST48 SCNx48
#else
#define SCNdLEAST48 SCNdLEAST56
#define SCNiLEAST48 SCNiLEAST56
#define SCNoLEAST48 SCNoLEAST56
#define SCNuLEAST48 SCNuLEAST56
#define SCNxLEAST48 SCNxLEAST56
#endif
#ifdef SCNd56
#define SCNdLEAST56 SCNd56
#define SCNiLEAST56 SCNi56
#define SCNoLEAST56 SCNo56
#define SCNuLEAST56 SCNu56
#define SCNxLEAST56 SCNx56
#else
#define SCNdLEAST56 SCNdLEAST64
#define SCNiLEAST56 SCNiLEAST64
#define SCNoLEAST56 SCNoLEAST64
#define SCNuLEAST56 SCNuLEAST64
#define SCNxLEAST56 SCNxLEAST64
#endif
#define SCNdLEAST64 SCNd64
#define SCNiLEAST64 SCNi64
#define SCNoLEAST64 SCNo64
#define SCNuLEAST64 SCNu64
#define SCNxLEAST64 SCNx64

#define SCNdFAST8 SCNdLEAST8
#define SCNiFAST8 SCNiLEAST8
#define SCNoFAST8 SCNoLEAST8
#define SCNuFAST8 SCNuLEAST8
#define SCNxFAST8 SCNxLEAST8
#define SCNdFAST16 SCNdLEAST16
#define SCNiFAST16 SCNiLEAST16
#define SCNoFAST16 SCNoLEAST16
#define SCNuFAST16 SCNuLEAST16
#define SCNxFAST16 SCNxLEAST16
#define SCNdFAST24 SCNdLEAST24
#define SCNiFAST24 SCNiLEAST24
#define SCNoFAST24 SCNoLEAST24
#define SCNuFAST24 SCNuLEAST24
#define SCNxFAST24 SCNxLEAST24
#define SCNdFAST32 SCNdLEAST32
#define SCNiFAST32 SCNiLEAST32
#define SCNoFAST32 SCNoLEAST32
#define SCNuFAST32 SCNuLEAST32
#define SCNxFAST32 SCNxLEAST32
#define SCNdFAST40 SCNdLEAST40
#define SCNiFAST40 SCNiLEAST40
#define SCNoFAST40 SCNoLEAST40
#define SCNuFAST40 SCNuLEAST40
#define SCNxFAST40 SCNxLEAST40
#define SCNdFAST48 SCNdLEAST48
#define SCNiFAST48 SCNiLEAST48
#define SCNoFAST48 SCNoLEAST48
#define SCNuFAST48 SCNuLEAST48
#define SCNxFAST48 SCNxLEAST48
#define SCNdFAST56 SCNdLEAST56
#define SCNiFAST56 SCNiLEAST56
#define SCNoFAST56 SCNoLEAST56
#define SCNuFAST56 SCNuLEAST56
#define SCNxFAST56 SCNxLEAST56
#define SCNdFAST64 SCNdLEAST64
#define SCNiFAST64 SCNiLEAST64
#define SCNoFAST64 SCNoLEAST64
#define SCNuFAST64 SCNuLEAST64
#define SCNxFAST64 SCNxLEAST64

#define SCNdMAX "jd"
#define SCNiMAX "ji"
#define SCNoMAX "jo"
#define SCNuMAX "ju"
#define SCNxMAX "jx"

#define SCNdPTR __INTPTR_FMTd__
#define SCNiPTR __INTPTR_FMTi__
#define SCNoPTR __UINTPTR_FMTo__
#define SCNuPTR __UINTPTR_FMTu__
#define SCNxPTR __UINTPTR_FMTx__

__BEGIN_DECLS
intmax_t imaxabs(intmax_t) __pure2;
imaxdiv_t imaxdiv(intmax_t, intmax_t) __pure2;
intmax_t strtoimax(const char *__restrict, char **__restrict, int);
intmax_t strtoimax_l(const char *__restrict, char **__restrict, int,
                     __locale_t);
uintmax_t strtoumax(const char *__restrict, char **__restrict, int);
uintmax_t strtoumax_l(const char *__restrict, char **__restrict, int,
                      __locale_t);
intmax_t wcstoimax(const wchar_t *__restrict, wchar_t **__restrict, int);
uintmax_t wcstoumax(const wchar_t *__restrict, wchar_t **__restrict, int);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline intmax_t __imaxabs(intmax_t __i) {
  return __i < 0 ? -__i : __i;
}
#define imaxabs(i) __imaxabs(i)

static __inline imaxdiv_t __imaxdiv(intmax_t __numer, intmax_t __denom) {
  imaxdiv_t __res = {__numer / __denom, __numer % __denom};
  return __res;
}
#define imaxdiv(numer, denom) __imaxdiv(numer, denom)
#endif

#endif
