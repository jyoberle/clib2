// Checks the ACrypt function
#include <exec/types.h>
#include <proto/exec.h>
#include <clib/alib_protos.h>
#include <string.h>

extern struct ExecBase *SysBase;

#define PASSWORD_BUFFER_SIZE 16

UBYTE pw[] = "password";
UBYTE new_pw[] = "IthmiWAB^Cs";
UBYTE user[] = "alf";
UBYTE *newpass;
UBYTE buffer[PASSWORD_BUFFER_SIZE] ;

WORD testACryptFunction(void)
{
	WORD errCnt = 0;
	UBYTE i;

	if((newpass = ACrypt(buffer, pw, user)) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(strlen(newpass) != strlen(new_pw))
		{
			errCnt++;
		}
		else
		{
			for(i=0;i < strlen(newpass);i++)
			{
				if(newpass[i] != new_pw[i])
				{
					errCnt++;
					break;
				}
			}
		}
	}

	return(errCnt);
}