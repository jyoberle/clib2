/*
 * $Id: stdlib_umodsi3.c,v 1.3 2006-01-08 12:04:26 obarthel Exp $
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

__attribute__((externally_visible)) ULONG __umodsi3(ULONG dividend,ULONG divisor)
{
	register volatile ULONG _d0                ASM("d0") = dividend;
	register volatile ULONG _d1                ASM("d1") = divisor; // also contains return value
	register volatile struct UtilityBase * _a0 ASM("a0") = UtilityBase;
	__asm volatile (
		"jsr -156(%%a0)\n" // UDivMod32
	: "=r"(_d0),"=r"(_d1) // the utility.library math routines preserve all	address registers including A0 and A1
	: "r"(_d0),"r"(_d1),"r"(_a0)
	: "fp0", "fp1", "cc", "memory");
	return((ULONG)_d1); // remainder in D1
}
#else
__asm("							\n\
	.text						\n\
	.align 2					\n\
								\n\
	.globl __umodsi3			\n\
								\n\
__umodsi3:						\n\
	divwu	r0,r3,r4			\n\
 	mullw	r0,r0,r4			\n\
 	subf	r3,r0,r3			\n\
	blr							\n\
");

#endif
/****************************************************************************/

#endif /* __GNUC__ */
