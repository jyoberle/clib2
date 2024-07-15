/*
 * $Id: math_tgamma.c,v 1.3 2006-01-08 12:04:24 obarthel Exp $
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2015 by Olaf Barthel <obarthel (at) gmx.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Neither the name of Olaf Barthel nor the names of contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * PowerPC math library based in part on work by Sun Microsystems
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 */

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

/****************************************************************************/

#if defined(FLOATING_POINT_SUPPORT)

/****************************************************************************/

double
tgamma(double x)
{
	// modified by JOB
    double y;

	// If x is ±0, tgamma(), tgammaf(), and tgammal() shall return ±HUGE_VAL, ±HUGE_VALF, and ±HUGE_VALL, respectively.
	// On systems that support the IEC 60559 Floating-Point option, a pole error shall occur; otherwise, a pole error may occur.
	if((fpclassify(x) == FP_ZERO) && (signbit(x) == 0))
	{
		__set_errno(ERANGE); // pole error
		return(__inf());
	}

	if((fpclassify(x) == FP_ZERO) && (signbit(x) != 0))
	{
		__set_errno(ERANGE); // pole error
		return(-__inf());
	}

	// If x is NaN, a NaN shall be returned.
	if(isnan(x))
		return(nan(NULL));

	if(isinf(x))
	{
		// If x is +Inf, x shall be returned.
		if(x > 0)
			return(__inf());

		// x is -infinity
		// If x is -Inf, a domain error shall occur, and a NaN shall be returned.
		__set_errno(EDOM);
    	return(nan(NULL));
	}

	y = exp(lgamma(x));

	return y;
}

/****************************************************************************/

#endif /* FLOATING_POINT_SUPPORT */
