/*
 * $Id: math_divdf3.c,v 1.3 2006-01-08 12:04:23 obarthel Exp $
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

#if defined(IEEE_FLOATING_POINT_SUPPORT) && defined(__GNUC__)

/****************************************************************************/

#if defined(SMALL_DATA)
#define A4(x) "a4@(" #x ":W)"
#elif defined(SMALL_DATA32)
#define A4(x) "a4@(" #x ":L)"
#else
#define A4(x) #x
#endif /* SMALL_DATA */

/****************************************************************************/

// modified by JOB
extern struct Library *MathIeeeDoubBasBase;

__attribute__((externally_visible)) double __divdf3(double a,double b)
{
	volatile double result_zero; // to avoid optimisation from the compiler

	if(isnan(a) || isnan(b))
		return(nan(NULL));

	if((isinf(a) && isinf(b)) 
		|| ((fpclassify(a) == FP_ZERO) && (fpclassify(b) == FP_ZERO)))
	{
		__set_errno(EDOM);
		return(nan(NULL));
	}

	if(isinf(a))
	{
		if(signbit(a) == 0)
		{
			if(signbit(b) == 0)
				return(__inf()); // +infinity

			// b < 0
			return(-__inf()); // -infinity
		}

		// a is -infinity
		if(signbit(b) == 0)
			return(-__inf()); // -infinity

		// b < 0
		return(__inf()); // +infinity
	}

	if(isinf(b))
	{
		if(signbit(b) == 0)
		{
			if(signbit(a) == 0)
				return(0.0);

			// a < 0
			result_zero = -0.0;
			return(result_zero);
		}

		// b is -infinity
		if(signbit(a) == 0)
		{
			result_zero = -0.0;
			return(result_zero);
		}

		return(0.0);
	}

	if((fpclassify(b) == FP_ZERO) && (signbit(b) == 0)) // b is 0.0
	{
		if(signbit(a) == 0)
			return(__inf());

		// a < 0
		return(-__inf());
	}

	if((fpclassify(b) == FP_ZERO) && (signbit(b) != 0)) // b is -0.0
	{
		if(signbit(a) == 0)
			return(-__inf());

		// a < 0
		return(__inf());
	}

	return(IEEEDPDiv(a,b));
}

/****************************************************************************/

#endif /* IEEE_FLOATING_POINT_SUPPORT */
