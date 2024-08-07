/*
 * $Id: math_addsf3.c,v 1.3 2006-01-08 12:04:23 obarthel Exp $
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

__attribute__((externally_visible)) float
__addsf3(float x,float y)
{
	float result;

	if(isnan(x) || isnan(y))
		return(nanf(NULL));

	if(isinf(x) && isinf(y))
	{
		if(signbit(x) == 0)
		{
			if(signbit(y) == 0)
				return(__inff()); // +infinity

			// y is -infinity
			__set_errno(EDOM);
			return(nanf(NULL));
		}

		// x is -infinity
		if(signbit(y) == 0)
		{
			__set_errno(EDOM);
			return(nanf(NULL));				
		}

		// y is -infinity
		return(-__inff()); // -infinity
	}

	if(isinf(x))
	{
		if(signbit(x)== 0)
			return(__inff());

		// x is -infinity
		return(-__inff());
	}

	if(isinf(y))
	{
		if(signbit(y) == 0)
			return(__inff());

		// y is -infinity
		return(-__inff());
	}

	result = IEEESPAdd(x,y);

	return(result);
}

/****************************************************************************/

#endif /* IEEE_FLOATING_POINT_SUPPORT */
