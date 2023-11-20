#if defined(__GNUC__)

/****************************************************************************/

#include <proto/utility.h>
#define ASM __asm

extern struct UtilityBase *UtilityBase;

__attribute__((externally_visible)) LONG __moddi3(LONG dividend,LONG divisor)
{
	register volatile LONG _d0                 ASM("d0") = dividend;
	register volatile LONG _d1                 ASM("d1") = divisor; // also contains return value
	register volatile struct UtilityBase * _a0 ASM("a0") = UtilityBase;
	__asm volatile (
		"jsr -150(%%a0)\n" // _LVOSDivMod32
	: "=r"(_d0),"=r"(_d1) // the utility.library math routines preserve all	address registers including A0 and A1
	: "r"(_d0),"r"(_d1),"r"(_a0)
	: "fp0", "fp1", "cc", "memory");
	return((LONG)_d1); // remainder in D1
}

/****************************************************************************/

#endif /* __GNUC__ */