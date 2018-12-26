#include "libm.h"
#include <cloudlibc_interceptors.h>

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double __cloudlibc_frexpl(long double x, int *e)
{
	return frexp(x, e);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
long double __cloudlibc_frexpl(long double x, int *e)
{
	union ldshape u = {x};
	int ee = u.i.se & 0x7fff;

	if (!ee) {
		if (x) {
			x = frexpl(x*0x1p120, e);
			*e -= 120;
		} else *e = 0;
		return x;
	} else if (ee == 0x7fff) {
		return x;
	}

	*e = ee - 0x3ffe;
	u.i.se &= 0x8000;
	u.i.se |= 0x3ffe;
	return u.f;
}
#endif

__weak_reference(__cloudlibc_frexpl, frexpl);
