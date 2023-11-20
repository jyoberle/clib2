/*
 * $Id: amiga_setsuperattrs.c,v 1.5 2006-01-08 12:04:22 obarthel Exp $
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

#ifndef __PPC__

/****************************************************************************/

#include "l_alib.h"
#include <proto/exec.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>

#include <clib/alib_protos.h>
#include <stdarg.h>

/****************************************************************************/

#include "debug.h"

/****************************************************************************/

STATIC ULONG
SetSuperAttrsA(Class * cl,Object * obj,struct TagItem * tags)
{
	ULONG result = 0;

	ENTER();

	SHOWPOINTER(cl);
	SHOWPOINTER(obj);
	SHOWPOINTER(tags);

	assert( cl != NULL && obj != NULL );

	if(cl != NULL && obj != NULL)
	{
		struct opSet ops;

		ops.MethodID		= OM_SET;
		ops.ops_AttrList	= tags;
		ops.ops_GInfo		= NULL;

		result = CallHookA(&cl->cl_Super->cl_Dispatcher,obj,&ops);
	}

	RETURN(result);
	return(result);
}

/****************************************************************************/

// moodified by JOB
ULONG SetSuperAttrs(Class *cl,Object *obj,ULONG tag1, ...)
{
    ULONG result = 0L;
	va_list param;
	ULONG *tags;
	int i;

    if(cl != NULL && obj != NULL)
    {
		if ((tags = AllocVec(ARG_CNT*sizeof(ULONG),MEMF_ANY)))
		{
			tags[0] = tag1;
			va_start(param,tag1);
			
			for(int i=1;i<ARG_CNT;i++) {
				tags[i] = (ULONG)va_arg(param,struct TagItem *);
			}

			va_end(param);

			result = SetSuperAttrsA(cl,obj,(struct TagItem *)tags);
			FreeVec(tags);
		}
    }

    return(result);
}

/*ULONG
SetSuperAttrs(Class * cl,Object * obj,ULONG tag1,...)
{
	ULONG result = 0;

	ENTER();

	SHOWPOINTER(cl);
	SHOWPOINTER(obj);
	SHOWVALUE(tag1);

	assert( cl != NULL && obj != NULL );

	if(cl != NULL && obj != NULL)
		result = SetSuperAttrsA(cl,obj,(struct TagItem *)&tag1);

	RETURN(result);
	return(result);
}*/

/****************************************************************************/

#endif /* __PPC__ */
