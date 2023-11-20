/*
 * $Id: stdlib_divsi3.c,v 1.3 2006-01-08 12:04:25 obarthel Exp $
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

#if defined(__GNUC__)

/****************************************************************************/
#ifndef __PPC__
#include <proto/utility.h>

#define ASM __asm

// modified by JOB
extern struct UtilityBase *UtilityBase;

__attribute__((externally_visible)) LONG __divsi3(LONG dividend,LONG divisor)
{
	register volatile LONG _d0                ASM("d0") = dividend; // also contains return value
	register volatile LONG _d1                ASM("d1") = divisor;
	register volatile struct UtilityBase * _a0 ASM("a0") = UtilityBase;
	__asm volatile (
		"jsr -150(%%a0)\n" // SDivMod32
	: "=r"(_d0),"=r"(_d1) // the utility.library math routines preserve all	address registers including A0 and A1
	: "r"(_d0),"r"(_d1),"r"(_a0)
	: "fp0", "fp1", "cc", "memory");
	return((LONG)_d0); // quotient in D0
}
#else
__asm ("						\n\
	 .text						\n\
	 .align 2					\n\
								\n\
	 .globl __divsi3			\n\
								\n\
__divsi3:						\n\
     divw  r3, r3, r4			\n\
	 blr						\n\
");
#endif

/****************************************************************************/

#endif /* __GNUC__ */
