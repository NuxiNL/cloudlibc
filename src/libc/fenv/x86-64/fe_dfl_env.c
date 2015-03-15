// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>

const fenv_t __fe_dfl_env = {
    // Initial x87 state as set by the finit instruction.
    .__x87 =
        {
            .__control = 0xffff037f,
            .__status = 0xffff0000,
            .__tag = 0xffffffff,
            .__fpu_ipo = 0x00000000,
            .__fpu_ips = 0x00000000,
            .__fpu_dpo = 0x00000000,
            .__fpu_dps = 0xffff0000,
        },

    // Initial SSE state.
    .__mxcsr = 0x1f80,
};
