/*
 * $Id: math_gesf2.c,v 1.3 2006-01-08 12:04:23 obarthel Exp $
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

// modified by JOB
// __gesf2 returns a value greater than or equal to zero if neither argument is NaN, and a is greater than or equal to b. 
__attribute__((externally_visible)) LONG
__gesf2(float x,float y)
{
	LONG result;

	if(isnan(x) || isnan(y))
		return(-1L);

	if(isinf(x) && isinf(y))
	{
		if(signbit(x) == 0)
		{
			if(signbit(y) == 0)
				return(0L);

			// y is -infinity
			return(1L);
		}

		// x is -infinity
		if(signbit(y) == 0)
			return(-1L);
		
		// y is -infinity
		return(1L);
	}

	if(isinf(x))
	{
		if(signbit(x) == 0)
			return(1L);

		// x is -infinity
		return(-1L);
	}

	if(isinf(y))
	{
		if(signbit(y) == 0)
			return(-1L);

		// y is -infinity
		return(1L);
	}

	result = IEEESPCmp(x,y);

	return(result);
}

/****************************************************************************/

#endif /* IEEE_FLOATING_POINT_SUPPORT */
