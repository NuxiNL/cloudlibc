// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

static const struct lc_time time_ru_ru = {
    .d_t_fmt = L"%A, %e %B %Y г. %H:%M:%S",
    .d_fmt = L"%d.%m.%Y",
    .t_fmt = L"%H:%M:%S",
    .t_fmt_ampm = L"",
    .am_str = L"",
    .pm_str = L"",
    .day =
        {
            L"воскресенье",
            L"понедельник",
            L"вторник",
            L"среда",
            L"четверг",
            L"пятница",
            L"суббота",
        },
    .abday =
        {
            L"вс",
            L"пн",
            L"вт",
            L"ср",
            L"чт",
            L"пт",
            L"сб",
        },
    .mon = {L"января", L"февраля", L"марта", L"апреля", L"мая", L"июня",
            L"июля", L"августа", L"сентября", L"октября", L"ноября",
            L"декабря"},
    .abmon = {L"янв", L"фев", L"мар", L"апр", L"май", L"июн", L"июл", L"авг",
              L"сен", L"окт", L"ноя", L"дек"},
};
