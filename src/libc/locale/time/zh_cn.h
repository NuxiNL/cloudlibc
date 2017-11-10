// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

static const struct lc_time time_zh_cn = {
    .d_t_fmt = L"%a %b/%e %H:%M:%S %Y",
    .d_fmt = L"%Y/%m/%d",
    .t_fmt = L"%H时%M分%S秒",
    .t_fmt_ampm = L"%I:%M:%S %p",
    .am_str = L"上午",
    .pm_str = L"下午",
    .day = {L"星期日", L"星期一", L"星期二", L"星期三", L"星期四", L"星期五",
            L"星期六"},
    .abday = {L"日", L"一", L"二", L"三", L"四", L"五", L"六"},
    .mon = {L"一月", L"二月", L"三月", L"四月", L"五月", L"六月", L"七月",
            L"八月", L"九月", L"十月", L"十一月", L"十二月"},
    .abmon = {L" 1", L" 2", L" 3", L" 4", L" 5", L" 6", L" 7", L" 8", L" 9",
              L"10", L"11", L"12"},
};
