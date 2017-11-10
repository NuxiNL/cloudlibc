// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

static const struct lc_time time_fr_fr = {
    .d_t_fmt = L"%a %e %b %H:%M:%S %Y",
    .d_fmt = L"%d.%m.%Y",
    .t_fmt = L"%H:%M:%S",
    .t_fmt_ampm = L"",
    .am_str = L"",
    .pm_str = L"",
    .day = {L"dimanche", L"lundi", L"mardi", L"mercredi", L"jeudi", L"vendredi",
            L"samedi"},
    .abday = {L"dim", L"lun", L"mar", L"mer", L"jeu", L"ven", L"sam"},
    .mon = {L"janvier", L"février", L"mars", L"avril", L"mai", L"juin",
            L"juillet", L"août", L"septembre", L"octobre", L"novembre",
            L"décembre"},
    .abmon = {L"jan", L"fév", L"mar", L"avr", L"mai", L"jui", L"jul", L"aoû",
              L"sep", L"oct", L"nov", L"déc"},
};
