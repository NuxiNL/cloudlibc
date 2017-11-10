// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/refcount.h>

#include <locale.h>

struct __locale __locale_c_unicode = {
    .refcount = REFCOUNT_INIT(1),

    .collate = &__collate_posix,
    .ctype = &__ctype_utf_8,
    .messages = &__messages_en_us,
    .monetary = &__monetary_posix,
    .numeric = &__numeric_posix,
    .time = &__time_posix,
    .timezone = &__timezone_utc,
};
