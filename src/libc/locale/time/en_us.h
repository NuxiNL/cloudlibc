// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

static const struct lc_time time_en_us = {
    .d_t_fmt = L"%a %b %e %H:%M:%S %Y",
    .d_fmt = L"%m/%d/%Y",
    .t_fmt = L"%H:%M:%S",
    .t_fmt_ampm = L"%I:%M:%S %p",
    .am_str = L"AM",
    .pm_str = L"PM",
    .day = {L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday",
            L"Friday", L"Saturday"},
    .abday = {L"Sun", L"Mon", L"Tue", L"Wed", L"Thu", L"Fri", L"Sat"},
    .mon = {L"January", L"February", L"March", L"April", L"May", L"June",
            L"July", L"August", L"September", L"October", L"November",
            L"December"},
    .abmon = {L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug",
              L"Sep", L"Oct", L"Nov", L"Dec"},
};
