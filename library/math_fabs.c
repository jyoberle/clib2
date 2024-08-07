/*
 * $Id: math_fabs.c,v 1.8 2006-01-08 12:04:23 obarthel Exp $
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
 */

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

/****************************************************************************/

#if defined(FLOATING_POINT_SUPPORT)

/****************************************************************************/

#if defined(IEEE_FLOATING_POINT_SUPPORT)

/****************************************************************************/

#if defined(__GNUC__)

/****************************************************************************/

#if defined(SMALL_DATA)
#define A4(x) "a4@(" #x ":W)"
#elif defined(SMALL_DATA32)
#define A4(x) "a4@(" #x ":L)"
#else
#define A4(x) #x
#endif /* SMALL_DATA */

/****************************************************************************/

extern double __fabs(double x);

/****************************************************************************/

// modified by JOB
extern struct Library *MathIeeeDoubBasBase;

__attribute__((externally_visible)) double __fabs(double a)
{
	return(IEEEDPAbs(a));
}

/****************************************************************************/

#else

/****************************************************************************/

INLINE STATIC const double
__fabs(double x)
{
	double result;

	result = IEEEDPAbs(x);

	return(result);
}

/****************************************************************************/

#endif /* __GNUC__ */

/****************************************************************************/

#endif /* IEEE_FLOATING_POINT_SUPPORT */

/****************************************************************************/

#if defined(M68881_FLOATING_POINT_SUPPORT)

INLINE STATIC const double
__fabs(double x)
{
	double result;

	__asm ("fabs%.x %1,%0"
	       : "=f" (result)
	       : "f" (x));

	return(result);
}

#endif /* M68881_FLOATING_POINT_SUPPORT */

/****************************************************************************/

#if defined(__PPC__)

/****************************************************************************/

#if defined(PPC_FLOATING_POINT_SUPPORT)

/****************************************************************************/

INLINE STATIC double
__fabs(double x)
{
	double res;

	__asm volatile ("fabs %0, %1"
					: "=f" (res)
					: "f" (x));

	return res;

}

/****************************************************************************/

#else

/****************************************************************************/

INLINE STATIC const double
__fabs(double x)
{
	double res;

	if(x < 0)
		res = (-x);
	else
		res = x;

	return res;
}

/****************************************************************************/

#endif /* PPC_FLOATING_POINT_SUPPORT */

/****************************************************************************/

#endif /* __PPC__ */

/****************************************************************************/

// If x is NaN, a NaN shall be returned.
// If x is ±0, +0 shall be returned.
// If x is ±Inf, +Inf shall be returned.
double
fabs(double x)
{
	double result;

	if(isnan(x))
		return(nan(NULL));

	if(isinf(x))
		return(__inf()); // +infinity

	result = __fabs(x);

	return(result);
}

/****************************************************************************/

#endif /* FLOATING_POINT_SUPPORT */
