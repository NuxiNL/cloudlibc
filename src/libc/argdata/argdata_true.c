// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <stdint.h>

static uint8_t buf_true[] = {ADT_BOOL, 1};

const argdata_t argdata_true = {
    .type = AD_BUFFER, .buffer = buf_true, .length = sizeof(buf_true),
};
