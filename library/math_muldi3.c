// created by JOB
/****************************************************************************/
#if defined(__GNUC__)
#ifndef __PPC__
#include <proto/utility.h>

extern struct UtilityBase *UtilityBase;

__attribute__((externally_visible)) LONG __muldi3(LONG Arg1,LONG Arg2)
{
     return(SMult32(Arg1,Arg2));
}
#endif
/****************************************************************************/
#endif /* __GNUC__ */
