#if defined(__GNUC__)

/****************************************************************************/

#include <exec/types.h>

#define ASM __asm

// This function returns the result of arithmetically shifting a left by b bits. 
__attribute__((externally_visible)) LONG __ashldi3(LONG a,ULONG b)
{
	register volatile LONG _d0 ASM("d0") = a;
	register volatile ULONG _d1 ASM("d1") = b;
	__asm volatile (
		"ASL.L %%d1,%%d0 \n"
	: "=r"(_d0)
	: "r"(_d0),"r"(_d1)
	: "fp0", "fp1", "cc", "memory");
	return(_d0);
}


/****************************************************************************/

#endif /* __GNUC__ */