#if defined(__GNUC__)

/****************************************************************************/

#include <exec/types.h>

#define ASM __asm

// This function returns the result of logically shifting a right by b bits. 
__attribute__((externally_visible)) ULONG __lshrdi3(ULONG a,ULONG b)
{
	register volatile ULONG _d0 ASM("d0") = a;
	register volatile ULONG _d1 ASM("d1") = b;
	__asm volatile (
		"LSR.L #8,%%d0 \n"
	: "=r"(_d0)
	: "r"(_d0),"r"(_d1)
	: "fp0", "fp1", "cc", "memory");
	return(_d0);
}


/****************************************************************************/

#endif /* __GNUC__ */