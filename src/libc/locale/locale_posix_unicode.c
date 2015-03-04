// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>
#include <common/refcount.h>

#include <locale.h>

struct __locale __locale_posix_unicode = {
    .refcount = REFCOUNT_INIT(1),

    .collate = &__collate_posix,
    .ctype = &__ctype_utf_8,
    .messages = &__messages_en_us,
    .monetary = &__monetary_posix,
    .numeric = &__numeric_posix,
    .time = &__time_en_us,
    .timezone = &__timezone_utc,
};
