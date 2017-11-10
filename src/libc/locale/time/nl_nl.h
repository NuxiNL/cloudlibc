// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

static const struct lc_time time_nl_nl = {
    .d_t_fmt = L"%a %e %b %H:%M:%S %Y",
    .d_fmt = L"%d-%m-%Y",
    .t_fmt = L"%H:%M:%S",
    .t_fmt_ampm = L"%I:%M:%S %p",
    .am_str = L"a.m.",
    .pm_str = L"p.m.",
    .day = {L"zondag", L"maandag", L"dinsdag", L"woensdag", L"donderdag",
            L"vrijdag", L"zaterdag"},
    .abday = {L"zo", L"ma", L"di", L"wo", L"do", L"vr", L"za"},
    .mon = {L"januari", L"februari", L"maart", L"april", L"mei", L"juni",
            L"juli", L"augustus", L"september", L"oktober", L"november",
            L"december"},
    .abmon = {L"jan", L"feb", L"mrt", L"apr", L"mei", L"jun", L"jul", L"aug",
              L"sep", L"okt", L"nov", L"dec"},
};
