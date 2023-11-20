// Checks the fast floating point functions: afp, fpa, dbf, arnd, fpbcd
#include <exec/types.h>
#include <proto/exec.h>
#include <exec/tasks.h>
#include <proto/dos.h>
#define FLOAT ULONG
#include <clib/alib_protos.h>

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

#define FFP_STR_LEN 14
#define FFP_BCD_LEN 8

static BYTE ffp_result1[FFP_STR_LEN+1],ffp_result2[FFP_STR_LEN+1],ffp_result3[FFP_STR_LEN+1],ffp_result4[FFP_STR_LEN+1],ffp_result5[FFP_STR_LEN+1],ffp_result6[FFP_STR_LEN+1];
static BYTE bcd_result1[FFP_BCD_LEN],bcd_result2[FFP_BCD_LEN],bcd_result3[FFP_BCD_LEN],bcd_result4[FFP_BCD_LEN],bcd_result5[FFP_BCD_LEN];
static const UBYTE expected_bcd_result1[FFP_BCD_LEN] = {0x12,0x00,0x00,0x00,0x00,0x03,0x00,0x03};
static const UBYTE expected_bcd_result2[FFP_BCD_LEN] = {0x31,0x41,0x59,0x27,0x00,0x01,0x00,0x01};
static const UBYTE expected_bcd_result3[FFP_BCD_LEN] = {0x12,0x00,0x00,0x00,0xff,0x03,0x00,0x03};
static const UBYTE expected_bcd_result4[FFP_BCD_LEN] = {0x10,0x00,0x00,0x00,0xff,0x10,0x00,0x0a};
static const UBYTE expected_bcd_result5[FFP_BCD_LEN] = {0x10,0x00,0x00,0x01,0x00,0x02,0xff,0xfe}; // 0x01 due to rounding of afp

static BOOL fcntByteCmp(const BYTE *b1, const BYTE *b2,WORD len)
{
	WORD i;

	for(i=0;i < len;i++)
	{
		if(b1[i] != b2[i])
			return(FALSE);
	}

	return(TRUE);
}

#if OPTION_TRACE
VOID fcntDumpHex(BYTE *ptr,WORD len)
{
	for(WORD i=0;i < FFP_BCD_LEN;i++)
		Printf("%lX ",ptr[i]);

	Printf("\n");
}
#endif

WORD testFFP(void)
{
	FLOAT ffp1,ffp2,ffp3,ffp4,ffp5;
	LONG exp1,exp2,exp3,exp4,exp5,exp6;
	WORD errCnt = 0;

	// Verify afp and fpa
	ffp1 = afp("+1.25E3");
	ffp2 = afp("+3.14159E-10");
	exp1 = fpa(ffp1,ffp_result1);
	exp2 = fpa(ffp2,ffp_result2);

#if OPTION_TRACE
	ffp_result1[FFP_STR_LEN] = '\0';
	ffp_result2[FFP_STR_LEN] = '\0';
	Printf("ffp1 = %s\n",(ULONG)ffp_result1);
	Printf("ffp2 = %s\n",(ULONG)ffp_result2);
	Printf("exp1 = %ld\n",(LONG)exp1);
	Printf("exp2 = %ld\n",(LONG)exp2);
#endif
	
	if(!fcntByteCmp(ffp_result1,"+.12500000E+04",FFP_STR_LEN)) errCnt++;
	if(!fcntByteCmp(ffp_result2,"+.31415901E-09",FFP_STR_LEN)) errCnt++;
	if(exp1 != 4L) errCnt++;
	if(exp2 != -9L) errCnt++;

	// Verify dbf
	ffp1 = dbf(10L,325L);
	ffp2 = dbf(8L | (1 << 16),42L); // negative
	fpa(ffp1,ffp_result1);
	fpa(ffp2,ffp_result2);


#if OPTION_TRACE
	Printf("dbf1 = %s\n",(ULONG)ffp_result1);
	Printf("dbf2 = %s\n",(ULONG)ffp_result2);
#endif

	if(!fcntByteCmp(ffp_result1,"+.32500001E+13",FFP_STR_LEN)) errCnt++;
	if(!fcntByteCmp(ffp_result2,"-.42000000E+10",FFP_STR_LEN)) errCnt++;

	// Verify arnd
	ffp1 = afp("+.98765432E+01");
	exp1 = fpa(ffp1,ffp_result1);
	exp2 = fpa(ffp1,ffp_result2);
	exp3 = fpa(ffp1,ffp_result3);
	exp4 = fpa(ffp1,ffp_result4);
	exp5 = fpa(ffp1,ffp_result5);
	exp6 = fpa(ffp1,ffp_result6);
	arnd(1L,exp1,ffp_result1);
	arnd(0L,exp2,ffp_result2);
	arnd(-1L,exp3,ffp_result3);
	arnd(-2L,exp4,ffp_result4);
	arnd(-3L,exp5,ffp_result5);
	arnd(-6L,exp6,ffp_result6);

	if(!fcntByteCmp(ffp_result1,"+.00000000E+00",FFP_STR_LEN)) errCnt++;
	if(!fcntByteCmp(ffp_result2,"+.10000000E+02",FFP_STR_LEN)) errCnt++;
	if(!fcntByteCmp(ffp_result3,"+.10000000E+02",FFP_STR_LEN)) errCnt++;
	if(!fcntByteCmp(ffp_result4,"+.99000000E+01",FFP_STR_LEN)) errCnt++;
	if(!fcntByteCmp(ffp_result5,"+.98800000E+01",FFP_STR_LEN)) errCnt++;
	if(!fcntByteCmp(ffp_result6,"+.98765400E+01",FFP_STR_LEN)) errCnt++;

#if OPTION_TRACE
	Printf("arnd1 = %s\n",(ULONG)ffp_result1);
	Printf("arnd2 = %s\n",(ULONG)ffp_result2);
	Printf("arnd3 = %s\n",(ULONG)ffp_result3);
	Printf("arnd4 = %s\n",(ULONG)ffp_result4);
	Printf("arnd5 = %s\n",(ULONG)ffp_result5);
	Printf("arnd6 = %s\n",(ULONG)ffp_result6);
#endif

	// Verify fpbcd
	ffp1 = afp("120");
	fpa(ffp1,ffp_result1);
	ffp2 = afp("3.1415927");
	fpa(ffp2,ffp_result2);
	ffp3 = afp("-120");
	fpa(ffp3,ffp_result3);
	ffp4 = afp("-1E+9");
	fpa(ffp4,ffp_result4);
	ffp5 = afp("0.1E-2");
	fpa(ffp5,ffp_result5);
	fpbcd(ffp1,bcd_result1);
	fpbcd(ffp2,bcd_result2);
	fpbcd(ffp3,bcd_result3);
	fpbcd(ffp4,bcd_result4);
	fpbcd(ffp5,bcd_result5);

#if OPTION_TRACE
	Printf("fpbcd1 = %s\n",(ULONG)ffp_result1);
	Printf("fpbcd2 = %s\n",(ULONG)ffp_result2);
	Printf("fpbcd3 = %s\n",(ULONG)ffp_result3);
	Printf("fpbcd4 = %s\n",(ULONG)ffp_result4);
	Printf("fpbcd5 = %s\n",(ULONG)ffp_result5);
	fcntDumpHex(bcd_result1,FFP_BCD_LEN);
	fcntDumpHex(bcd_result2,FFP_BCD_LEN);
	fcntDumpHex(bcd_result3,FFP_BCD_LEN);
	fcntDumpHex(bcd_result4,FFP_BCD_LEN);
	fcntDumpHex(bcd_result5,FFP_BCD_LEN);
#endif

	if(!fcntByteCmp(bcd_result1,expected_bcd_result1,FFP_BCD_LEN)) errCnt++;
	if(!fcntByteCmp(bcd_result2,expected_bcd_result2,FFP_BCD_LEN)) errCnt++;
	if(!fcntByteCmp(bcd_result3,expected_bcd_result3,FFP_BCD_LEN)) errCnt++;
	if(!fcntByteCmp(bcd_result4,expected_bcd_result4,FFP_BCD_LEN)) errCnt++;
	if(!fcntByteCmp(bcd_result5,expected_bcd_result5,FFP_BCD_LEN)) errCnt++;

	return(errCnt);
}