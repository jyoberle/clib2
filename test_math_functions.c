// Checks the mathlib support functions
// Most of the nan/inf conditions are taken from https://pubs.opengroup.org/onlinepubs/9699919799/
// Others from https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html
#include <stdio.h>
#include <stdio_headers.h>
#include <exec/types.h>
#include <proto/exec.h>
#include "math_headers.h"
#include "complex_headers.h"
#include "complex.h"

#define TEST_STRING_1 "In the beginning "
#define TEST_STRING_2 "God created the heaven and the Earth."
#define TEST_STRING_1_2 "In the beginning God created the heaven and the Earth."
#define LOVE_STRING "JOB loves Amiga"
#define RAM_TEST_FILE "RAM:test_file"
#define BUFFER_SIZE 256
#define NBR1 12
#define NBR2 24
#define OPTION_TRACE_MATH 0

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;
extern struct Library *MathIeeeDoubBasBase;
extern double __adddf3(double a,double b);
extern float __addsf3(float a,float b);
extern double __divdf3(double a,double b);
extern float __divsf3(float a,float b);
extern LONG __eqdf2(double a,double b);
extern LONG __eqsf2(float a,float b);
extern double __extendsfdf2(float a);
extern LONG __fixdfsi(double a);
extern signed long __fixsfsi(float a);
extern unsigned long __fixunsdfsi(double a);
extern unsigned long __fixunssfsi(float a);
extern double __floatsidf(LONG a);
extern float __floatsisf(signed long x);
extern LONG __gedf2(double a,double b);
extern LONG __gesf2(float a,float b);
extern LONG __gtdf2(double a,double b);
extern LONG __gtsf2(float a,float b);
extern LONG __ledf2(double a,double b);
extern LONG __lesf2(float a,float b);
extern LONG __ltdf2(double a,double b);
extern LONG __ltsf2(float a,float b);
extern double __muldf3(double a,double b);
extern float __mulsf3(float a,float b);
extern LONG __nedf2(double a,double b);
extern LONG __nesf2(float a,float b);
extern double __negdf2(double a);
extern float __negsf2(float a);
extern double __subdf3(double a,double b);
extern float __subsf3(float a,float b);
extern float __truncdfsf2(double a);
extern ULONG __udivsi3(ULONG dividend,ULONG divisor);
extern LONG __divsi3(LONG dividend,LONG divisor);
extern LONG __mulsi3(LONG Arg1,LONG Arg2);
extern float __floatunsisf(ULONG a);
extern double __floatunsidf(ULONG a);
extern LONG __ashldi3(LONG a,ULONG b);
extern ULONG __lshrdi3(ULONG a,ULONG b);
extern LONG __divdi3(LONG dividend,LONG divisor);
extern LONG __moddi3(LONG dividend,LONG divisor);
extern LONG __muldi3(LONG Arg1,LONG Arg2);
extern LONG __clzsi2(ULONG x);
extern LONG __ctzsi2(ULONG x);

static const char *hexStrTable[256] = {
	"00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
	"10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
	"20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
	"30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
	"40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
	"50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
	"60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
	"70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
	"80","81","82","83","84","85","86","87","88","89","8A","8B","8C","8D","8E","8F",
	"90","91","92","93","94","95","96","97","98","99","9A","9B","9C","9D","9E","9F",
	"A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","AA","AB","AC","AD","AE","AF",
	"B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA","BB","BC","BD","BE","BF",
	"C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB","CC","CD","CE","CF",
	"D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC","DD","DE","DF",
	"E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED","EE","EF",
	"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE","FF",
};

static char buffer1[BUFFER_SIZE],buffer2[BUFFER_SIZE],test_strings[BUFFER_SIZE];
static FILE *file_ptr;

// To test vasprintf
__attribute__((noinline)) int math_vasprintf_test(char **ret, const char *format, ...)
{
	int result = EOF;
	va_list arg;

	va_start(arg,format);
	result = vasprintf(ret,format,arg);
	va_end(arg);

	return(result);
}

// To test vfprintf
__attribute__((noinline)) int math_vfprintf_test(FILE *file_ptr,const char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vfprintf(file_ptr,format,arg);
	va_end(arg);

	return(result);
}

// To test vscanf
__attribute__((noinline)) int math_vscanf_test(char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vscanf(format,arg);
	va_end(arg);

	return(result);
}

// To test vfscanf
__attribute__((noinline)) int math_vfscanf_test(FILE *file_ptr,const char *format, ...)
{
	int result = EOF;
	va_list arg;

	va_start(arg,format);
	result = vfscanf(file_ptr,format,arg);
	va_end(arg);

	return(result);
}

// To test vprintf
__attribute__((noinline)) int math_vprintf_test(const char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vprintf(format,arg);
	va_end(arg);

	return(result);
}

// To test vsnprintf
__attribute__((noinline)) int math_vsnprintf_test(char *s,size_t n,char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vsnprintf(s,n,format,arg);
	va_end(arg);

	return(result);
}

// To test vsprintf
__attribute__((noinline)) int math_vsprintf_test(char *s,char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vsprintf(s,format,arg);
	va_end(arg);

	return(result);
}

// To test vsscanf
__attribute__((noinline)) int math_vsscanf_test(char *s,char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vsscanf(s,format,arg);
	va_end(arg);

	return(result);
}

static BOOL fcntDoubleInRange(double x,double y)
{
	const double delta = 0.001;

	if(fabs(x - y) < delta)
		return(TRUE);

	return(FALSE);	
}

static BOOL fcntFloatInRange(float x,float y)
{
	const float delta = 0.001f;

	if(fabsf(x - y) < delta)
		return(TRUE);

	return(FALSE);	
}

#if USE_LONG_DOUBLE
static BOOL fcntLongDoubleInRange(long double x,long double y)
{
	const long double delta = 0.001;

	if(fabsl(x - y) < delta)
		return(TRUE);

	return(FALSE);	
}
#endif

WORD testMathFunctions(void)
{
	WORD errCnt = 0;
	double vald;
	float valf;

	// Test creal, cimag, carg, conj
	double complex z1 = -3.14+9.69*I;

	if(!fcntDoubleInRange(creal(z1),-3.14))
		errCnt++;

	if(!fcntDoubleInRange(cimag(z1),9.69))
		errCnt++;

	if(!fcntDoubleInRange(carg(z1),1.8841))
		errCnt++;

	if(!fcntDoubleInRange(creal(conj(z1)),-3.14))
		errCnt++;

	if(!fcntDoubleInRange(cimag(conj(z1)),-9.69))
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE1 errCnt = %u\n",errCnt);
#endif

	// Test crealf, cimagf, cargf, conjf
	float complex z2 = -3.14+9.69*I;

	if(!fcntFloatInRange(crealf(z2),-3.14))
		errCnt++;

	if(!fcntFloatInRange(cimagf(z2),9.69))
		errCnt++;

	if(!fcntFloatInRange(cargf(z2),1.8841))
		errCnt++;

	if(!fcntFloatInRange(crealf(conjf(z2)),-3.14))
		errCnt++;

	if(!fcntFloatInRange(cimagf(conjf(z2)),-9.69))
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE2 errCnt = %u\n",errCnt);
#endif

#if USE_LONG_DOUBLE // long double is not supported
	// Test creall, cimagl, cargl, conjl
	long double complex z3 = -3.14+9.69*I;

	if(!fcntLongDoubleInRange(creall(z3),-3.14))
		errCnt++;

	if(!fcntLongDoubleInRange(cimagl(z3),9.69))
		errCnt++;

	if(!fcntLongDoubleInRange(cargl(z3),1.8841))
		errCnt++;

	if(!fcntLongDoubleInRange(creall(conjl(z3)),-3.14))
		errCnt++;

	if(!fcntLongDoubleInRange(cimagl(conjl(z3)),-9.69))
		errCnt++;
#endif

	// Test feclearexcept
	if(feclearexcept(FE_ALL_EXCEPT) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE3 errCnt = %u\n",errCnt);
#endif

	// Test fegetenv
	fenv_t envp;

	if(fegetenv(&envp) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE4 errCnt = %u\n",errCnt);
#endif

	// Test fegetenv
	fexcept_t flagp;

	if(fegetexceptflag(&envp,FE_DIVBYZERO) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE5 errCnt = %u\n",errCnt);
#endif

	// Test fegetround
	if(fegetround() != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE6 errCnt = %u\n",errCnt);
#endif

	// Test feholdexcept
	if(feholdexcept(&envp) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE7 errCnt = %u\n",errCnt);
#endif

	// Test feraiseexcept
	if(feraiseexcept(FE_DIVBYZERO) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE8 errCnt = %u\n",errCnt);
#endif

	// Test fesetenv
	if(fesetenv(&envp) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE9 errCnt = %u\n",errCnt);
#endif

	// Test fesetexceptflag
	if(fesetexceptflag(&flagp,FE_DIVBYZERO) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE10 errCnt = %u\n",errCnt);
#endif

	// Test fetestexcept
	if(fetestexcept(FE_DIVBYZERO) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE11 errCnt = %u\n",errCnt);
#endif

	// Test fetestround
	if(fetestround(FE_DOWNWARD) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE12 errCnt = %u\n",errCnt);
#endif

	// Test fetestround
	if(feupdateenv(&envp) != 0) // not implemented
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE13 errCnt = %u\n",errCnt);
#endif

	// Test acos, acosf, acosh, acoshf
	// For finite values of x not in the range [-1,1], a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntDoubleInRange(acos(0.5),1.04719)) errCnt++;
	if(!isnan(acos(nan(NULL)))) errCnt++;
	if(!isnan(acos(__inf()))) errCnt++;
	if(!isnan(acos(-__inf()))) errCnt++;
	if(!isnan(acos(1.5))) errCnt++;
	if(!isnan(acos(-1.5))) errCnt++;
	// For finite values of x not in the range [-1,1], a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntFloatInRange(acosf(0.5f),1.04719f)) errCnt++;
	if(!isnan(acosf(nanf(NULL)))) errCnt++;
	if(!isnan(acosf(__inff()))) errCnt++;
	if(!isnan(acosf(-__inff()))) errCnt++;
	if(!isnan(acosf(1.5))) errCnt++;
	if(!isnan(acosf(-1.5))) errCnt++;
	// For finite values of x < 1, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is +Inf, +Inf shall be returned
	// If x is -Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntDoubleInRange(acosh(1.256),0.70109)) errCnt++;
	if(!isnan(acosh(nan(NULL)))) errCnt++;
	vald = acosh(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	if(!isnan(acosh(-__inf()))) errCnt++;
	if(!isnan(acosh(0.99))) errCnt++;
	if(!isnan(acosh(-0.01))) errCnt++;
	// For finite values of x < 1, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is +Inf, +Inf shall be returned
	// If x is -Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntFloatInRange(acoshf(1.256f),0.70109f)) errCnt++;
	if(!isnan(acoshf(nanf(NULL)))) errCnt++;
	valf = acoshf(__inff());
	if(!isinf(valf) || signbit(vald) != 0) errCnt++;
	if(!isnan(acoshf(-__inff()))) errCnt++;
	if(!isnan(acoshf(0.99f))) errCnt++;
	if(!isnan(acoshf(-0.01f))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE14 errCnt = %u\n",errCnt);
#endif

	// Test asin, asin, asinh, asinhf
	// For finite values of x not in the range [-1,1], a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	// For finite values of x not in the range [-1,1], a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntDoubleInRange(asin(0.5),0.5235)) errCnt++;
	if(!isnan(asin(nan(NULL)))) errCnt++;
	if(!isnan(asin(__inf()))) errCnt++;
	if(!isnan(asin(-__inf()))) errCnt++;
	if(!isnan(asin(1.1))) errCnt++;
	if(!isnan(asin(-1.1))) errCnt++;
	// For finite values of x not in the range [-1,1], a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	// For finite values of x not in the range [-1,1], a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntFloatInRange(asinf(0.5f),0.5235f)) errCnt++;
	if(!isnan(asinf(nanf(NULL)))) errCnt++;
	if(!isnan(asinf(__inff()))) errCnt++;
	if(!isnan(asinf(-__inff()))) errCnt++;
	if(!isnan(asinf(1.1))) errCnt++;
	if(!isnan(asinf(-1.1))) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, or ±Inf, x shall be returned.
	if(!fcntDoubleInRange(asinh(1.256),1.0513)) errCnt++;
	if(!isnan(asinh(nan(NULL)))) errCnt++;
	vald = asinh(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = asinh(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, or ±Inf, x shall be returned.
	if(!fcntFloatInRange(asinhf(1.256f),1.0513f)) errCnt++;
	if(!isnan(asinhf(nanf(NULL)))) errCnt++;
	valf = asinhf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = asinhf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE15 errCnt = %u\n",errCnt);
#endif

	// Test atan, atan, atanh, atanhf
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, ±pi/2 shall be returned.
	if(!fcntDoubleInRange(atan(0.5),0.4636)) errCnt++;
	if(!isnan(atan(nan(NULL)))) errCnt++;
	if(!fcntDoubleInRange(atan(__inf()),M_PI_2)) errCnt++;
	if(!fcntDoubleInRange(atan(-__inf()),-M_PI_2)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, ±pi/2 shall be returned.
	if(!fcntFloatInRange(atanf(0.5f),0.4636f)) errCnt++;
	if(!isnan(atanf(nanf(NULL)))) errCnt++;
	if(!fcntFloatInRange(atanf(__inff()),(float)M_PI_2)) errCnt++;
	if(!fcntFloatInRange(atanf(-__inff()),(float)(-M_PI_2))) errCnt++;
	// If x is ±1, a pole error shall occur, and atanh(), atanhf(), and atanhl() shall return the value of the macro HUGE_VAL, HUGE_VALF, and HUGE_VALL, respectively, with the same sign as the correct value of the function.
	// For finite |x|>1, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntDoubleInRange(atanh(0.256),0.2618)) errCnt++;
	if(!isnan(atanh(nan(NULL)))) errCnt++;
	if(!isnan(atanh(__inf()))) errCnt++;
	if(!isnan(atanh(-__inf()))) errCnt++;
	vald = atanh(1.0f);
	if(!isinf(vald) && signbit(vald) != 0) errCnt++;
	vald = atanh(-1.0f);
	if(!isinf(vald) && signbit(vald) == 0) errCnt++;
	// If x is ±1, a pole error shall occur, and atanh(), atanhf(), and atanhl() shall return the value of the macro HUGE_VAL, HUGE_VALF, and HUGE_VALL, respectively, with the same sign as the correct value of the function.
	// For finite |x|>1, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntFloatInRange(atanhf(0.256f),0.2618f)) errCnt++;
	if(!isnan(atanhf(nanf(NULL)))) errCnt++;
	if(!isnan(atanhf(__inff()))) errCnt++;
	if(!isnan(atanhf(-__inff()))) errCnt++;
	valf = atanhf(1.0f);
	if(!isinf(valf) && signbit(valf) != 0) errCnt++;
	valf = atanhf(-1.0f);
	if(!isinf(valf) && signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE16 errCnt = %u\n",errCnt);
#endif

	// Test atan2, atan2f
	// If either x or y is NaN, a NaN shall be returned.
	// If y is ±0 and x is +0, ±0 shall be returned.
	// For finite values of ± y > 0, if x is -Inf, ± shall be returned.
	// For finite values of ± y > 0, if x is +Inf, ±0 shall be returned.
	// For finite values of x, if y is ±Inf, ±/2 shall be returned.
	// If y is ±Inf and x is -Inf, ±3/4 shall be returned.
	// If y is ±Inf and x is +Inf, ±/4 shall be returned.
	if(!fcntDoubleInRange(atan2(1.0,1.0),0.7853)) errCnt++;
	if(!fcntDoubleInRange(atan2(-1.0,-1.0),-2.3561)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0,-1.0),3.141593)) errCnt++;
	if(!fcntDoubleInRange(atan2(5.0000,6.0000),0.694738)) errCnt++;
	if(!fcntDoubleInRange(atan2(6.0000,5.0000),0.876058)) errCnt++;
	if(!fcntDoubleInRange(atan2(-5.0000,6.0000),-0.694738)) errCnt++;
	if(!fcntDoubleInRange(atan2(6.0000,-5.0000),2.265535)) errCnt++;
	if(!fcntDoubleInRange(atan2(-6.0000,5.0000),-0.876058)) errCnt++;
	if(!fcntDoubleInRange(atan2(5.0000,-6.0000),2.446854)) errCnt++; 
	if(!fcntDoubleInRange(atan2(-5.0000,-6.0000),-2.446854)) errCnt++;
	if(!fcntDoubleInRange(atan2(-6.0000,-5.0000),-2.265535)) errCnt++;
	if(!fcntDoubleInRange(atan2(6.0000,6.0000),0.785398)) errCnt++;
	if(!fcntDoubleInRange(atan2(-6.0000,6.0000),-0.785398)) errCnt++;
	if(!fcntDoubleInRange(atan2(6.0000,-6.0000),2.356194)) errCnt++;
	if(!fcntDoubleInRange(atan2(-6.0000,-6.0000),-2.356194)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0000,0.0000),0.0)) errCnt++;
	if(!fcntDoubleInRange(atan2(1.0000,0.0000),1.570796)) errCnt++;
	if(!fcntDoubleInRange(atan2(2.0000,0.0000),1.570796)) errCnt++;
	if(!fcntDoubleInRange(atan2(3.0000,0.0000),1.570796)) errCnt++;
	if(!fcntDoubleInRange(atan2(-1.0000,0.0000),-1.570796)) errCnt++;
	if(!fcntDoubleInRange(atan2(-2.0000,0.0000),-1.570796)) errCnt++;
	if(!fcntDoubleInRange(atan2(-3.0000,0.0000),-1.570796)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0000,1.0000),0.0)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0000,2.0000),0.0)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0000,3.0000),0.0)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0000,-1.0000),3.141593)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0000,-2.0000),3.141593)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0000,-3.0000),3.141593)) errCnt++;
	if(!isnan(atan2(nan(NULL),nan(NULL)))) errCnt++;
	if(!isnan(atan2(nan(NULL),3.14159))) errCnt++;
	if(!isnan(atan2(3.14159,nan(NULL)))) errCnt++;
	if(!fcntDoubleInRange(atan2(__inf(),__inf()),M_PI_4)) errCnt++;
	if(!fcntDoubleInRange(atan2(-__inf(),__inf()),-M_PI_4)) errCnt++;
	if(!fcntDoubleInRange(atan2(__inf(),-__inf()),3.0*M_PI_4)) errCnt++;
	if(!fcntDoubleInRange(atan2(-__inf(),-__inf()),-3.0*M_PI_4)) errCnt++;
	if(!fcntDoubleInRange(atan2(1.0,__inf()),0.0)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0,__inf()),0.0)) errCnt++;
	if(!fcntDoubleInRange(atan2(-1.0,__inf()),-0.0)) errCnt++;
	if(!fcntDoubleInRange(atan2(1.0,-__inf()),M_PI)) errCnt++;
	if(!fcntDoubleInRange(atan2(0.0,-__inf()),M_PI)) errCnt++;
	if(!fcntDoubleInRange(atan2(-1.0,-__inf()),-M_PI)) errCnt++;
	if(!fcntDoubleInRange(atan2(__inf(),1.0),M_PI_2)) errCnt++;
	if(!fcntDoubleInRange(atan2(-__inf(),1.0),-M_PI_2)) errCnt++;
	// If either x or y is NaN, a NaN shall be returned.
	// If y is ±0 and x is +0, ±0 shall be returned.
	// For finite values of ± y > 0, if x is -Inf, ± shall be returned.
	// For finite values of ± y > 0, if x is +Inf, ±0 shall be returned.
	// For finite values of x, if y is ±Inf, ±/2 shall be returned.
	// If y is ±Inf and x is -Inf, ±3/4 shall be returned.
	// If y is ±Inf and x is +Inf, ±/4 shall be returned.
	if(!fcntFloatInRange(atan2f(1.0f,1.0f),0.7853f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-1.0f,-1.0f),-2.3561f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0f,-1.0f),3.141593f)) errCnt++;
	if(!fcntFloatInRange(atan2f(5.0000f,6.0000f),0.694738f)) errCnt++;
	if(!fcntFloatInRange(atan2f(6.0000f,5.0000f),0.876058f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-5.0000f,6.0000f),-0.694738f)) errCnt++;
	if(!fcntFloatInRange(atan2f(6.0000f,-5.0000f),2.265535f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-6.0000f,5.0000f),-0.876058f)) errCnt++;
	if(!fcntFloatInRange(atan2f(5.0000f,-6.0000f),2.446854f)) errCnt++; 
	if(!fcntFloatInRange(atan2f(-5.0000f,-6.0000f),-2.446854f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-6.0000f,-5.0000f),-2.265535f)) errCnt++;
	if(!fcntFloatInRange(atan2f(6.0000f,6.0000f),0.785398f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-6.0000f,6.0000f),-0.785398f)) errCnt++;
	if(!fcntFloatInRange(atan2f(6.0000f,-6.0000f),2.356194f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-6.0000f,-6.0000f),-2.356194f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0000f,0.0000f),0.0f)) errCnt++;
	if(!fcntFloatInRange(atan2f(1.0000f,0.0000f),1.570796f)) errCnt++;
	if(!fcntFloatInRange(atan2f(2.0000f,0.0000f),1.570796f)) errCnt++;
	if(!fcntFloatInRange(atan2f(3.0000f,0.0000f),1.570796f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-1.0000f,0.0000f),-1.570796f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-2.0000f,0.0000f),-1.570796f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-3.0000f,0.0000f),-1.570796f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0000f,1.0000f),0.0f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0000f,2.0000f),0.0f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0000f,3.0000f),0.0f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0000f,-1.0000f),3.141593f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0000f,-2.0000f),3.141593f)) errCnt++;
	if(!fcntFloatInRange(atan2f(0.0000f,-3.0000f),3.141593f)) errCnt++;
	if(!isnan(atan2f(nanf(NULL),nanf(NULL)))) errCnt++;
	if(!isnan(atan2f(nanf(NULL),3.14159f))) errCnt++;
	if(!isnan(atan2f(3.14159f,nanf(NULL)))) errCnt++;
	if(!fcntDoubleInRange(atan2f(__inff(),__inff()),(float)M_PI_4)) errCnt++;
	if(!fcntDoubleInRange(atan2f(-__inff(),__inff()),(float)(-M_PI_4))) errCnt++;
	if(!fcntDoubleInRange(atan2f(__inff(),-__inff()),(float)(3.0*M_PI_4))) errCnt++;
	if(!fcntDoubleInRange(atan2f(-__inff(),-__inff()),(float)(-3.0*M_PI_4))) errCnt++;
	if(!fcntDoubleInRange(atan2f(1.0f,__inff()),0.0f)) errCnt++;
	if(!fcntDoubleInRange(atan2f(0.0f,__inff()),0.0f)) errCnt++;
	if(!fcntDoubleInRange(atan2f(-1.0f,__inff()),-0.0f)) errCnt++;
	if(!fcntDoubleInRange(atan2f(1.0f,-__inff()),(float)M_PI)) errCnt++;
	if(!fcntDoubleInRange(atan2f(0.0f,-__inff()),(float)M_PI)) errCnt++;
	if(!fcntDoubleInRange(atan2f(-1.0f,-__inff()),(float)(-M_PI))) errCnt++;
	if(!fcntDoubleInRange(atan2f(__inff(),1.0f),(float)M_PI_2)) errCnt++;
	if(!fcntDoubleInRange(atan2f(-__inff(),1.0f),(float)(-M_PI_2))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE17 errCnt = %u\n",errCnt);
#endif

	// Test cbrt, cbrtf
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntDoubleInRange(cbrt(1969.1969),12.5341)) errCnt++;
	if(!isnan(cbrt(nan(NULL)))) errCnt++;
	vald = cbrt(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = cbrt(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntFloatInRange(cbrtf(1969.1969f),12.5341f)) errCnt++;
	if(!isnan(cbrtf(nanf(NULL)))) errCnt++;
	valf = cbrtf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = cbrtf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE18 errCnt = %u\n",errCnt);
#endif

	// Test ceil, ceilf
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntDoubleInRange(ceil(1971.1971),1972.0)) errCnt++;
	if(!isnan(ceil(nan(NULL)))) errCnt++;
	vald = ceil(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = ceil(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntFloatInRange(ceilf(1971.1971f),1972.0f)) errCnt++;
	if(!isnan(ceilf(nanf(NULL)))) errCnt++;
	valf = ceilf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = ceilf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE19 errCnt = %u\n",errCnt);
#endif

	// Test copysign, copysignf
	if(!fcntDoubleInRange(copysign(1971.1971,-1969.0),-1971.1971)) errCnt++;
	if(!fcntDoubleInRange(copysign(1971.1971,1969.0),1971.1971)) errCnt++;
	if(!fcntFloatInRange(copysignf(1971.1971f,-1969.0f),-1971.1971f)) errCnt++;
	if(!fcntFloatInRange(copysignf(1971.1971f,1969.0f),1971.1971f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE20 errCnt = %u\n",errCnt);
#endif

	// Test cos, cosf, cosh, coshf
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntDoubleInRange(cos(0.5),0.8775)) errCnt++;
	if(!isnan(cos(nan(NULL)))) errCnt++;
	if(!isnan(cos(__inf()))) errCnt++;
	if(!isnan(cos(-__inf()))) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntFloatInRange(cosf(0.5f),0.8775f)) errCnt++;
	if(!isnan(cosf(nanf(NULL)))) errCnt++;
	if(!isnan(cosf(__inff()))) errCnt++;
	if(!isnan(cosf(-__inff()))) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	if(!fcntDoubleInRange(cosh(1.256),1.8980)) errCnt++;
	if(!isnan(cosh(nan(NULL)))) errCnt++;
	vald = cosh(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = cosh(-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	if(!fcntFloatInRange(coshf(1.256f),1.8980f)) errCnt++;
	if(!isnan(coshf(nanf(NULL)))) errCnt++;
	valf = coshf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = coshf(-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE21 errCnt = %u\n",errCnt);
#endif

	// Test erf, erff, erf, erfcf
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, ±1 shall be returned.
	if(!fcntDoubleInRange(erf(0.5),0.5205)) errCnt++;
	if(!isnan(erf(nan(NULL)))) errCnt++;
	if(erf(__inf()) != 1.0) errCnt++;
	if(erf(-__inf()) != -1.0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, ±1 shall be returned.
	if(!fcntFloatInRange(erff(0.5f),0.5205f)) errCnt++;
	if(!isnan(erff(nanf(NULL)))) errCnt++;
	if(erff(__inff()) != 1.0f) errCnt++;
	if(erff(-__inff()) != -1.0f) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, +1 shall be returned.
	// If x is -Inf, +2 shall be returned.
	// If x is +Inf, +0 shall be returned.
	if(!fcntDoubleInRange(erfc(0.5),0.4795)) errCnt++;
	if(!isnan(erfc(nan(NULL)))) errCnt++;
	if(erfc(0.0) != 1.0) errCnt++;
	if(erfc(-0.0) != 1.0) errCnt++;
	vald = erfc(__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	if(erfc(-__inf()) != 2.0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, +1 shall be returned.
	// If x is -Inf, +2 shall be returned.
	// If x is +Inf, +0 shall be returned.
	if(!fcntFloatInRange(erfcf(0.5f),0.4795f)) errCnt++;
	if(!isnan(erfcf(nanf(NULL)))) errCnt++;
	if(erfcf(0.0f) != 1.0f) errCnt++;
	if(erfcf(-0.0f) != 1.0f) errCnt++;
	valf = erfcf(__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	if(erfcf(-__inff()) != 2.0f) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE22 errCnt = %u\n",errCnt);
#endif

	// Test exp, expf, exp2, exp2f
	// If x is NaN, a NaN shall be returned.
	// If x is -Inf, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	if(!fcntDoubleInRange(exp(3.14159),23.1406)) errCnt++;
	if(!isnan(exp(nan(NULL)))) errCnt++;
	vald = exp(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = exp(-__inf());
	if(fpclassify(0.0) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is -Inf, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	if(!fcntFloatInRange(expf(3.14159f),23.1406f)) errCnt++;
	if(!isnan(expf(nanf(NULL)))) errCnt++;
	valf = expf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = expf(-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, 1 shall be returned.
	// If x is -Inf, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	if(!fcntDoubleInRange(exp2(3.14159),8.8249)) errCnt++;
	if(!isnan(exp2(nan(NULL)))) errCnt++;
	vald = exp2(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = exp2(-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, 1 shall be returned.
	// If x is -Inf, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	if(!fcntFloatInRange(exp2f(3.14159f),8.8249f)) errCnt++;
	if(!isnan(exp2f(nanf(NULL)))) errCnt++;
	valf = exp2f(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = exp2f(-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE23 errCnt = %u\n",errCnt);
#endif

	// Test expm1, expm1f
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, ±0 shall be returned.
	// If x is -Inf, -1 shall be returned.
	// If x is +Inf, x shall be returned.
	if(!fcntDoubleInRange(expm1(3.14159),22.1406)) errCnt++;
	if(!isnan(expm1(nan(NULL)))) errCnt++;
	vald = expm1(0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = expm1(-0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = expm1(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	if(expm1(-__inf()) != -1.0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, ±0 shall be returned.
	// If x is -Inf, -1 shall be returned.
	// If x is +Inf, x shall be returned.
	if(!fcntFloatInRange(expm1f(3.14159f),22.1406f)) errCnt++;
	if(!isnan(expm1f(nanf(NULL)))) errCnt++;
	valf = expm1f(0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = expm1f(-0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = expm1f(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	if(expm1f(-__inff()) != -1.0f) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE24 errCnt = %u\n",errCnt);
#endif

	// Test fabs, fabsf
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, +0 shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	if(!fcntDoubleInRange(fabs(-3.14159),3.14159)) errCnt++;
	if(!fcntDoubleInRange(fabs(3.14159),3.14159)) errCnt++;
	if(!isnan(fabs(nan(NULL)))) errCnt++;
	vald = fabs(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = fabs(-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, +0 shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	if(!fcntFloatInRange(fabsf(-3.14159f),3.14159f)) errCnt++;
	if(!fcntFloatInRange(fabsf(3.14159f),3.14159f)) errCnt++;
	if(!isnan(fabsf(nanf(NULL)))) errCnt++;
	valf = fabsf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = fabsf(-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE25 errCnt = %u\n",errCnt);
#endif

	// Test fdim, fdimf
	// If x or y is NaN, a NaN shall be returned.
	if(!fcntDoubleInRange(fdim(3.6,3.5),0.1)) errCnt++;
	if(!fcntDoubleInRange(fdim(3.5,3.6),0.0)) errCnt++;
	if(!isnan(fdim(nan(NULL),3.14159))) errCnt++;
	if(!isnan(fdim(3.14159,nan(NULL)))) errCnt++;
	if(!isnan(fdim(nan(NULL),nan(NULL)))) errCnt++;
	// If x or y is NaN, a NaN shall be returned.
	if(!fcntFloatInRange(fdimf(3.6f,3.5f),0.1f)) errCnt++;
	if(!fcntFloatInRange(fdimf(3.5f,3.6f),0.0f)) errCnt++;
	if(!isnan(fdimf(nanf(NULL),3.14159f))) errCnt++;
	if(!isnan(fdimf(3.14159f,nanf(NULL)))) errCnt++;
	if(!isnan(fdimf(nanf(NULL),nanf(NULL)))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE26 errCnt = %u\n",errCnt);
#endif

	// Test floor, floorf
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntDoubleInRange(floor(0.5),0.0)) errCnt++;
	if(!fcntDoubleInRange(floor(-0.5),-1.0)) errCnt++;
	if(!isnan(floor(nan(NULL)))) errCnt++;
	vald = floor(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = floor(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntFloatInRange(floorf(0.5f),0.0f)) errCnt++;
	if(!fcntFloatInRange(floorf(-0.5f),-1.0f)) errCnt++;
	if(!isnan(floorf(nanf(NULL)))) errCnt++;
	valf = floorf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = floorf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE27 errCnt = %u\n",errCnt);
#endif

	// Test fma, fmaf
	// If x or y are NaN, a NaN shall be returned.
	// If x multiplied by y is an exact infinity and z is also an infinity but with the opposite sign, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If one of x and y is infinite, the other is zero, and z is not a NaN, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If one of x and y is infinite, the other is zero, and z is a NaN, a NaN shall be returned and a domain error may occur.
	// If x* y is not 0*Inf nor Inf*0 and z is a NaN, a NaN shall be returned.
	if(!fcntDoubleInRange(fma(-1.1,-2.2,3.3),5.72)) errCnt++;
	if(!isnan(fma(nan(NULL),4.2,4.2))) errCnt++;
	if(!isnan(fma(1.0,nan(NULL),4.2))) errCnt++;
	if(!isnan(fma(1.0,3.14159,nan(NULL)))) errCnt++;
	if(!isnan(fma(__inf(),0.0,nan(NULL)))) errCnt++;
	if(!isnan(fma(__inf(),__inf(),-__inf()))) errCnt++;
	if(!isnan(fma(-__inf(),__inf(),__inf()))) errCnt++;
	if(!isnan(fma(__inf(),-__inf(),__inf()))) errCnt++;
	if(!isnan(fma(-__inf(),-__inf(),-__inf()))) errCnt++;
	if(!isnan(fma(__inf(),0.0,3.14159))) errCnt++;
	if(!isnan(fma(-__inf(),0.0,3.14159))) errCnt++;
	if(!isnan(fma(0.0,__inf(),3.14159))) errCnt++;
	if(!isnan(fma(0.0,-__inf(),3.14159))) errCnt++;
	if(!isnan(fma(__inf(),0.0,nan(NULL)))) errCnt++;
	if(!isnan(fma(-__inf(),0.0,nan(NULL)))) errCnt++;
	if(!isnan(fma(0.0,__inf(),nan(NULL)))) errCnt++;
	if(!isnan(fma(0.0,-__inf(),nan(NULL)))) errCnt++;
	if(!isnan(fma(__inf(),0.0,nan(NULL)))) errCnt++;
	if(!isnan(fma(__inf(),3.14159,nan(NULL)))) errCnt++;
	if(!isnan(fma(-__inf(),3.14159,nan(NULL)))) errCnt++;
	if(!isnan(fma(3.14159,__inf(),nan(NULL)))) errCnt++;
	if(!isnan(fma(3.14159,-__inf(),nan(NULL)))) errCnt++;
	vald = fma(3.14159,__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = fma(-3.14159,__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = fma(__inf(),3.14159,3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = fma(__inf(),-3.14159,3.14159);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = fma(3.14159,3.14159,__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = fma(3.14159,3.14159,-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x or y are NaN, a NaN shall be returned.
	// If x multiplied by y is an exact infinity and z is also an infinity but with the opposite sign, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If one of x and y is infinite, the other is zero, and z is not a NaN, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	// If one of x and y is infinite, the other is zero, and z is a NaN, a NaN shall be returned and a domain error may occur.
	// If x* y is not 0*Inf nor Inf*0 and z is a NaN, a NaN shall be returned.
	if(!fcntFloatInRange(fmaf(-1.1f,-2.2f,3.3f),5.72f)) errCnt++;
	if(!isnan(fmaf(nanf(NULL),4.2f,4.2f))) errCnt++;
	if(!isnan(fmaf(1.0f,nanf(NULL),4.2f))) errCnt++;
	if(!isnan(fmaf(1.0f,3.14159f,nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(__inff(),0.0f,nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(__inff(),__inff(),-__inff()))) errCnt++;
	if(!isnan(fmaf(-__inff(),__inff(),__inff()))) errCnt++;
	if(!isnan(fmaf(__inff(),-__inff(),__inff()))) errCnt++;
	if(!isnan(fmaf(-__inff(),-__inff(),-__inff()))) errCnt++;
	if(!isnan(fmaf(__inff(),0.0f,3.14159f))) errCnt++;
	if(!isnan(fmaf(-__inff(),0.0f,3.14159f))) errCnt++;
	if(!isnan(fmaf(0.0f,__inff(),3.14159f))) errCnt++;
	if(!isnan(fmaf(0.0f,-__inff(),3.14159f))) errCnt++;
	if(!isnan(fmaf(__inff(),0.0f,nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(-__inff(),0.0f,nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(0.0f,__inff(),nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(0.0f,-__inff(),nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(__inff(),0.0f,nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(__inff(),3.14159f,nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(-__inff(),3.14159f,nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(3.14159f,__inff(),nanf(NULL)))) errCnt++;
	if(!isnan(fmaf(3.14159f,-__inff(),nanf(NULL)))) errCnt++;
	valf = fma(3.14159f,__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = fma(-3.14159f,__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = fma(__inff(),3.14159f,3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = fma(__inff(),-3.14159f,3.14159f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = fma(3.14159f,3.14159f,__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = fma(3.14159f,3.14159f,-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE28 errCnt = %u\n",errCnt);
#endif

	// Test fmax, fmaxf
	// If just one argument is a NaN, the other argument shall be returned.
	// If x and y are NaN, a NaN shall be returned.
	if(!fcntDoubleInRange(fmax(-1.1,-2.2),-1.1)) errCnt++;
	if(!isnan(fmax(nan(NULL),nan(NULL)))) errCnt++;
	if(fmax(nan(NULL),3.14159) != 3.14159) errCnt++;
	if(fmax(3.14159,nan(NULL)) != 3.14159) errCnt++;
	// If just one argument is a NaN, the other argument shall be returned.
	// If x and y are NaN, a NaN shall be returned.
	if(!fcntFloatInRange(fmaxf(-1.1f,-2.2f),-1.1f)) errCnt++;
	if(!isnan(fmaxf(nanf(NULL),nanf(NULL)))) errCnt++;
	if(fmaxf(nanf(NULL),3.14159f) != 3.14159f) errCnt++;
	if(fmaxf(3.14159f,nanf(NULL)) != 3.14159f) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE29 errCnt = %u\n",errCnt);
#endif

	// Test fmin, fminf
	// If just one argument is a NaN, the other argument shall be returned.
	// If x and y are NaN, a NaN shall be returned.
	if(!fcntDoubleInRange(fmin(-1.1,-2.2),-2.2)) errCnt++;
	if(!isnan(fmin(nan(NULL),nan(NULL)))) errCnt++;
	if(fmin(nan(NULL),3.14159) != 3.14159) errCnt++;
	if(fmin(3.14159,nan(NULL)) != 3.14159) errCnt++;
	// If just one argument is a NaN, the other argument shall be returned.
	// If x and y are NaN, a NaN shall be returned.
	if(!fcntFloatInRange(fminf(-1.1f,-2.2f),-2.2f)) errCnt++;
	if(!isnan(fminf(nanf(NULL),nanf(NULL)))) errCnt++;
	if(fminf(nanf(NULL),3.14159f) != 3.14159f) errCnt++;
	if(fminf(3.14159f,nanf(NULL)) != 3.14159f) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE30 errCnt = %u\n",errCnt);
#endif

	// Test fmod, fmodf
	// If x or y is NaN, a NaN shall be returned, and none of the conditions below shall be considered.
	// If y is zero, a domain error shall occur, and a NaN shall be returned.
	// If x is infinite, a domain error shall occur, and a NaN shall be returned.
	// If x is ±0 and y is not zero, ±0 shall be returned.
	// If x is not infinite and y is ±Inf, x shall be returned.
	if(!fcntDoubleInRange(fmod(1677216.25,8192.25),5997.25)) errCnt++;
	if(!isnan(fmod(nan(NULL),nan(NULL)))) errCnt++;
	if(!isnan(fmod(3.14159,0.0))) errCnt++;
	if(!isnan(fmod(__inf(),3.14159))) errCnt++;
	if(!isnan(fmod(-__inf(),3.14159))) errCnt++;
	if(fmod(3.14159,__inf()) != 3.14159) errCnt++;
	if(fmod(3.14159,-__inf()) != 3.14159) errCnt++;
	vald = fmod(0.0,3.14159);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = fmod(-0.0,3.14159);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	// If x or y is NaN, a NaN shall be returned, and none of the conditions below shall be considered.
	// If y is zero, a domain error shall occur, and a NaN shall be returned.
	// If x is infinite, a domain error shall occur, and a NaN shall be returned.
	// If x is ±0 and y is not zero, ±0 shall be returned.
	// If x is not infinite and y is ±Inf, x shall be returned.
	if(!fcntFloatInRange(fmodf(1677216.25f,8192.25f),5997.25f)) errCnt++;
	if(!isnan(fmodf(nanf(NULL),nanf(NULL)))) errCnt++;
	if(!isnan(fmodf(3.14159f,0.0f))) errCnt++;
	if(!isnan(fmodf(__inff(),3.14159f))) errCnt++;
	if(!isnan(fmodf(-__inff(),3.14159f))) errCnt++;
	if(fmodf(3.14159f,__inff()) != 3.14159f) errCnt++;
	if(fmodf(3.14159f,-__inff()) != 3.14159f) errCnt++;
	valf = fmodf(0.0f,3.14159f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = fmodf(-0.0f,3.14159f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE31 errCnt = %u\n",errCnt);
#endif

	// Test fpclassify
	if(fpclassify(0.0) != FP_ZERO) errCnt++;
	if(fpclassify(26.04) != FP_NORMAL) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE32 errCnt = %u\n",errCnt);
#endif

	// Test frexp, frexpf
	int exp;
	// If num is NaN, a NaN shall be returned, and the value of *exp is unspecified.
	// If num is ±0, ±0 shall be returned, and the value of *exp shall be 0.
	// If num is ±Inf, num shall be returned, and the value of *exp is unspecified.
	double fraction = frexp(2029.0,&exp);
	if(!fcntDoubleInRange(fraction,0.990723) || exp != 11) errCnt++;
	if(!isnan(frexp(nan(NULL),&exp))) errCnt++;
	vald = frexp(__inf(),&exp);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = frexp(-__inf(),&exp);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = frexp(0.0,&exp);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = frexp(-0.0,&exp);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	// If num is NaN, a NaN shall be returned, and the value of *exp is unspecified.
	// If num is ±0, ±0 shall be returned, and the value of *exp shall be 0.
	// If num is ±Inf, num shall be returned, and the value of *exp is unspecified.
	double fractionf = frexpf(2029.0f,&exp);
	if(!fcntFloatInRange(fractionf,0.990723f) || exp != 11) errCnt++;
	if(!isnan(frexpf(nanf(NULL),&exp))) errCnt++;
	valf = frexpf(__inff(),&exp);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = frexpf(-__inff(),&exp);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = frexpf(0.0f,&exp);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = frexpf(-0.0f,&exp);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE33 errCnt = %u\n",errCnt);
#endif

	// Test hypot, hypotf
	// If x or y is ±Inf, +Inf shall be returned (even if one of x or y is NaN).
	// If x or y is NaN, and the other is not ±Inf, a NaN shall be returned.
	if(!fcntDoubleInRange(hypot(11.25,9.33),14.6155)) errCnt++;
	vald = hypot(__inf(),nan(NULL));
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = hypot(-__inf(),nan(NULL));
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = hypot(nan(NULL),__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = hypot(nan(NULL),-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = hypot(3.14159,__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = hypot(3.14159,-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = hypot(__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = hypot(-__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	if(!isnan(hypot(nan(NULL),3.14159))) errCnt++;
	if(!isnan(hypot(3.14159,nan(NULL)))) errCnt++;
	// If x or y is ±Inf, +Inf shall be returned (even if one of x or y is NaN).
	// If x or y is NaN, and the other is not ±Inf, a NaN shall be returned.
	if(!fcntFloatInRange(hypotf(11.25f,9.33f),14.6155f)) errCnt++;
	valf = hypotf(__inff(),nanf(NULL));
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = hypotf(-__inff(),nanf(NULL));
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = hypotf(nanf(NULL),__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = hypotf(nanf(NULL),-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = hypotf(3.14159f,__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = hypotf(3.14159f,-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = hypotf(__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = hypotf(-__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	if(!isnan(hypotf(nanf(NULL),3.14159f))) errCnt++;
	if(!isnan(hypotf(3.14159f,nanf(NULL)))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE34 errCnt = %u\n",errCnt);
#endif

	// Test ilogb, ilogbf
	// If x is 0, the value FP_ILOGB0 shall be returned.
	// If x is ±Inf, the value {INT_MAX} shall be returned.
	// If x is a NaN, the value FP_ILOGBNAN shall be returned.
	if(ilogb(123.45) != 6) errCnt++;
	if(ilogb(0.0) != -INT_MAX) errCnt++;
	if(ilogb(__inf()) != INT_MAX) errCnt++;
	if(ilogb(-__inf()) != INT_MAX) errCnt++;
	if(ilogb(nan(NULL)) != INT_MAX) errCnt++;
	// If x is 0, the value FP_ILOGB0 shall be returned.
	// If x is ±Inf, the value {INT_MAX} shall be returned.
	// If x is a NaN, the value FP_ILOGBNAN shall be returned.
	if(ilogbf(123.45f) != 6) errCnt++;
	if(ilogbf(0.0f) != -INT_MAX) errCnt++;
	if(ilogbf(__inff()) != INT_MAX) errCnt++;
	if(ilogbf(-__inff()) != INT_MAX) errCnt++;
	if(ilogbf(nanf(NULL)) != INT_MAX) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE35 errCnt = %u\n",errCnt);
#endif

	// Test ldexp, ldexpf
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	// If exp is 0, x shall be returned.
	if(!fcntDoubleInRange(ldexp(1971.04,4),31536.64)) errCnt++;
	if(!isnan(ldexp(nan(NULL),31536.64))) errCnt++;
	vald = ldexp(__inf(),31536.64);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = ldexp(-__inf(),31536.64);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!fcntDoubleInRange(ldexp(1971.04,0),1971.04)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	// If exp is 0, x shall be returned.
	if(!fcntFloatInRange(ldexpf(1971.04f,4),31536.64f)) errCnt++;
	if(!isnan(ldexpf(nanf(NULL),31536.64f))) errCnt++;
	valf = ldexpf(__inff(),31536.64f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = ldexpf(-__inff(),31536.64f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	if(!fcntFloatInRange(ldexpf(1971.04f,0),1971.04f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE36 errCnt = %u\n",errCnt);
#endif

	// Test lgamma, lgammaf
	// If x is NaN, a NaN shall be returned.
	// If x is 1 or 2, +0 shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	// If x is a non-positive integer, a pole error shall occur and lgamma(), lgammaf(), and lgammal() shall return +HUGE_VAL, +HUGE_VALF, and +HUGE_VALL, respectively.
	if(!fcntDoubleInRange(lgamma(10.0),12.801827)) errCnt++;
	if(!isnan(lgamma(nan(NULL)))) errCnt++;
	vald = lgamma(1.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = lgamma(2.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = lgamma(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = lgamma(-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = lgamma(-4.0);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++; // lgamma returns +infinity
	// If x is NaN, a NaN shall be returned.
	// If x is 1 or 2, +0 shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	// If x is a non-positive integer, a pole error shall occur and lgamma(), lgammaf(), and lgammal() shall return +HUGE_VAL, +HUGE_VALF, and +HUGE_VALL, respectively.
	if(!fcntFloatInRange(lgammaf(10.0f),12.801827f)) errCnt++;
	if(!isnan(lgammaf(nanf(NULL)))) errCnt++;
	valf = lgammaf(1.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = lgammaf(2.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = lgammaf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = lgammaf(-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = lgammaf(-4.0f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++; // // lgamma returns +infinity

#if OPTION_TRACE_MATH
	printf("TRACE37 errCnt = %u\n",errCnt);
#endif

	// Test lrint, lrintf, llrint
	// If x is NaN, a domain error shall occur and an unspecified value is returned.
	// If x is +Inf, a domain error shall occur and an unspecified value is returned.
	// If x is -Inf, a domain error shall occur and an unspecified value is returned.
	if(lrint(1969.1976) != 1969UL) errCnt++;
	if(lrint(1969.9999) != 1970UL) errCnt++;
	if(lrint(nan(NULL)) != 0L) errCnt++;
	if(lrint(__inf()) != 0L) errCnt++;
	if(lrint(-__inf()) != 0L) errCnt++;
	// If x is NaN, a domain error shall occur and an unspecified value is returned.
	// If x is +Inf, a domain error shall occur and an unspecified value is returned.
	// If x is -Inf, a domain error shall occur and an unspecified value is returned.
	if(lrintf(1969.1976f) != 1969UL) errCnt++;
	if(lrintf(1969.9999f) != 1970UL) errCnt++;
	if(lrintf(nanf(NULL)) != 0L) errCnt++;
	if(lrintf(__inff()) != 0L) errCnt++;
	if(lrintf(-__inff()) != 0L) errCnt++;
#if defined(USE_64_BIT_INTS)
	if(llrint(1969.1976) != 1969ULL) errCnt++;
	if(llrint(1969.9999) != 1970ULL) errCnt++;
#endif

#if OPTION_TRACE_MATH
	printf("TRACE38 errCnt = %u\n",errCnt);
#endif

	// Test log, log10, log10f, log1p, log1pf, log2, log2f, logb, logbf, logf
	// If x is ±0, a pole error shall occur and log(), logf(), and logl() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than 0, or if x is -Inf, a domain error shall occur, and either a NaN or an implementation-defined value shall be returned.
	if(!fcntDoubleInRange(log(4.0),1.3862)) errCnt++;
	if(!isnan(log(nan(NULL)))) errCnt++;
	vald = log(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = log(0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = log(-0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!isnan(log(-4.0))) errCnt++;
	if(!isnan(log(-__inf()))) errCnt++;
	// If x is ±0, a pole error shall occur and log(), logf(), and logl() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than 0, or if x is -Inf, a domain error shall occur, and either a NaN or an implementation-defined value shall be returned.
	if(!fcntFloatInRange(logf(4.0f),1.3862f)) errCnt++;
	if(!isnan(logf(nanf(NULL)))) errCnt++;
	valf = logf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = logf(0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = logf(-0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	if(!isnan(logf(-4.0f))) errCnt++;
	if(!isnan(logf(-__inff()))) errCnt++;
	// If x is ±0, a pole error shall occur and log10(), log10f(), and log10l() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than 0, or if x is -Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntDoubleInRange(log10(4.0),0.6020)) errCnt++;
	if(!isnan(log10(nan(NULL)))) errCnt++;
	vald = log10(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = log10(0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = log10(-0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!isnan(log10(-4.0))) errCnt++;
	if(!isnan(log10(-__inf()))) errCnt++;
	// If x is ±0, a pole error shall occur and log10(), log10f(), and log10l() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than 0, or if x is -Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntFloatInRange(log10f(4.0f),0.6020f)) errCnt++;
	if(!isnan(log10f(nanf(NULL)))) errCnt++;
	valf = log10f(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = log10f(0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = log10f(-0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	if(!isnan(log10f(-4.0f))) errCnt++;
	if(!isnan(log10f(-__inff()))) errCnt++;
	// If x is -1, a pole error shall occur and log1p(), log1pf(), and log1pl() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than -1, or if x is -Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntDoubleInRange(log1p(4.0),1.6094)) errCnt++;
	if(!isnan(log1p(nan(NULL)))) errCnt++; 
	vald = log1p(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = log1p(-1.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++; 
	if(!isnan(log1p(-3.14159))) errCnt++; 
	if(!isnan(log1p(-__inf()))) errCnt++; 
	// If x is -1, a pole error shall occur and log1p(), log1pf(), and log1pl() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than -1, or if x is -Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntFloatInRange(log1pf(4.0f),1.6094f)) errCnt++;
	if(!isnan(log1pf(nanf(NULL)))) errCnt++;
	valf = log1pf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = log1pf(-1.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	if(!isnan(log1pf(-3.14159f))) errCnt++; 
	if(!isnan(log1pf(-__inff()))) errCnt++; 
	// If x is ±0, a pole error shall occur and log2(), log2f(), and log2l() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than 0, or if x is -Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntDoubleInRange(log2(6.0),2.58496)) errCnt++;
	if(!isnan(log2(nan(NULL)))) errCnt++;
	vald = log2(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = log2(0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = log2(-0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!isnan(log2(-4.0))) errCnt++;
	if(!isnan(log2(-__inf()))) errCnt++;
	// If x is ±0, a pole error shall occur and log2(), log2f(), and log2l() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// For finite values of x that are less than 0, or if x is -Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntFloatInRange(log2f(6.0f),2.58496f)) errCnt++;
	if(!isnan(log2f(nanf(NULL)))) errCnt++;
	valf = log2f(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = log2f(0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = log2f(-0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	if(!isnan(log2f(-4.0f))) errCnt++;
	if(!isnan(log2f(-__inff()))) errCnt++;
	// If x is ±0, logb(), logbf(), and logbl() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	if(!fcntDoubleInRange(logb(8.0),3.0)) errCnt++;
	if(!isnan(logb(nan(NULL)))) errCnt++;
	vald = logb(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = logb(-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = logb(0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = logb(-0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is ±0, logb(), logbf(), and logbl() shall return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
	// If x is NaN, a NaN shall be returned.
	// If x is 1, +0 shall be returned.
	// If x is +Inf, x shall be returned.
	// If x is ±Inf, +Inf shall be returned.
	if(!fcntFloatInRange(logbf(8.0f),3.0f)) errCnt++;
	if(!isnan(logbf(nanf(NULL)))) errCnt++;
	valf = logbf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = logbf(-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = logbf(0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = logbf(-0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE39 errCnt = %u\n",errCnt);
#endif

	// Test lround, lroundf
	//If x is NaN, a domain error shall occur and an unspecified value is returned.
	// If x is +Inf, a domain error shall occur and an unspecified value is returned.
	// If x is -Inf, a domain error shall occur and an unspecified value is returned.
	// If the correct value is positive and too large to represent as a long, an unspecified value shall be returned.
	// If the correct value is negative and too large to represent as a long, an unspecified value shall be returned.
	if(lround(4.9) != 5) errCnt++;
	if(lround(4.4) != 4) errCnt++;
	if(lround(nan(NULL)) != 0L) errCnt++;
	if(lround(__inf()) != 0L) errCnt++;
	if(lround(-__inf()) != 0L) errCnt++;
	//If x is NaN, a domain error shall occur and an unspecified value is returned.
	// If x is +Inf, a domain error shall occur and an unspecified value is returned.
	// If x is -Inf, a domain error shall occur and an unspecified value is returned.
	// If the correct value is positive and too large to represent as a long, an unspecified value shall be returned.
	// If the correct value is negative and too large to represent as a long, an unspecified value shall be returned.
	if(lroundf(4.9f) != 5) errCnt++;
	if(lroundf(4.4f) != 4) errCnt++;
	if(lroundf(nanf(NULL)) != 0L) errCnt++;
	if(lroundf(__inff()) != 0L) errCnt++;
	if(lroundf(-__inff()) != 0L) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE40 errCnt = %u\n",errCnt);
#endif

	// Test modf, modff
	double valmodf;
	float valmodff;
	// If x is NaN, a NaN shall be returned, and *iptr shall be set to a NaN.
	// If x is ±Inf, ±0 shall be returned, and *iptr shall be set to ±Inf.
	if(!fcntDoubleInRange(modf(4.159,&valmodf),0.159)) errCnt++;
	if(!fcntDoubleInRange(valmodf,4.0)) errCnt++;
	if(!isnan(modf(nan(NULL),&valmodf))) errCnt++;
	if(!isnan(valmodf)) errCnt++;
	vald = modf(__inf(),&valmodf);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	if(!isinf(valmodf) || signbit(valmodf) != 0) errCnt++;
	vald = modf(-__inf(),&valmodf);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	if(!isinf(valmodf) || signbit(valmodf) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned, and *iptr shall be set to a NaN.
	// If x is ±Inf, ±0 shall be returned, and *iptr shall be set to ±Inf.
	if(!fcntFloatInRange(modff(4.159f,&valmodff),0.159)) errCnt++;
	if(!fcntFloatInRange(valmodff,4.0f)) errCnt++;
	if(!isnan(modff(nanf(NULL),&valmodff))) errCnt++;
	if(!isnan(valmodff)) errCnt++;
	valf = modff(__inff(),&valmodff);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	if(!isinf(valmodff) || signbit(valmodff) != 0) errCnt++;
	valf = modff(-__inff(),&valmodff);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	if(!isinf(valmodff) || signbit(valmodff) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE41 errCnt = %u\n",errCnt);
#endif

	// Test nan, nanf
	if(fcntDoubleInRange(nan(NULL),0.0)) errCnt++; // argument is not used
	if(fcntFloatInRange(nanf(NULL),0.0f)) errCnt++; // argument is not used

#if OPTION_TRACE_MATH
	printf("TRACE42 errCnt = %u\n",errCnt);
#endif

	// Test nearbyint, nearbyintf
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, ±0 shall be returned.
	// If x is ±Inf, x shall be returned.
	if(!fcntDoubleInRange(nearbyint(4.159),4.0)) errCnt++;
	if(!fcntDoubleInRange(nearbyint(4.95),5.0)) errCnt++;
	if(!isnan(nearbyint(nan(NULL)))) errCnt++;
	vald = nearbyint(0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = nearbyint(-0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = nearbyint(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = nearbyint(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0, ±0 shall be returned.
	// If x is ±Inf, x shall be returned.
	if(!fcntFloatInRange(nearbyintf(4.159f),4.0f)) errCnt++;
	if(!fcntFloatInRange(nearbyintf(4.95f),5.0f)) errCnt++;
	if(!isnan(nearbyintf(nanf(NULL)))) errCnt++;
	valf = nearbyintf(0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = nearbyintf(-0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = nearbyintf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = nearbyintf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE43 errCnt = %u\n",errCnt);
#endif

	// Test nextafter, nextafterf
	// If x is finite and the correct function value would overflow, a range error shall occur and ±HUGE_VAL, ±HUGE_VALF, and ±HUGE_VALL (with the same sign as x)
	// If x or y is NaN, a NaN shall be returned.
	if(!fcntDoubleInRange(nextafter(1.0,1.1),1.000000001)) errCnt++; // fcntDoubleInRange does not check delta below 0.001
	if(!isnan(nextafter(nan(NULL),1.0))) errCnt++;
	if(!isnan(nextafter(1.0,nan(NULL)))) errCnt++;
	if(!fcntFloatInRange(nextafterf(1.0f,1.1f),1.000000001f)) errCnt++; // fcntFloatInRange does not check delta below 0.001f
	if(!isnan(nextafterf(nanf(NULL),1.0f))) errCnt++;
	if(!isnan(nextafterf(1.0f,nanf(NULL)))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE44 errCnt = %u\n",errCnt);
#endif

	// Test pow, powf
	if(!fcntDoubleInRange(pow(3.14159,3.14159),36.461952)) errCnt++;
	vald = pow(__inf(),__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = pow(__inf(),-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = pow(-__inf(),__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = pow(-__inf(),-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// For any value of y (including NaN), if x is +1, 1.0 shall be returned.
	if(pow(1.0,nan(NULL)) != 1.0) errCnt++;
	if(pow(1.0,__inf()) != 1.0) errCnt++;
	if(pow(1.0,-__inf()) != 1.0) errCnt++;
	// For any value of x (including NaN), if y is ±0, 1.0 shall be returned.
	if(pow(nan(NULL),0.0) != 1.0) errCnt++;
	if(pow(nan(NULL),-0.0) != 1.0) errCnt++;
	if(pow(__inf(),0.0) != 1.0) errCnt++;
	if(pow(__inf(),-0.0) != 1.0) errCnt++;
	if(pow(-__inf(),0.0) != 1.0) errCnt++;
	if(pow(-__inf(),-0.0) != 1.0) errCnt++;
	// If x or y is a NaN, a NaN shall be returned (unless specified elsewhere in this description).
	if(!isnan(pow(nan(NULL),3.14159))) errCnt++;
	if(!isnan(pow(3.14159,nan(NULL)))) errCnt++;
	if(!isnan(pow(nan(NULL),nan(NULL)))) errCnt++;
	// For any odd integer value of y > 0, if x is ±0, ±0 shall be returned.
	vald = pow(0.0,5.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = pow(-0.0,5.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	// For y > 0 and not an odd integer, if x is ±0, +0 shall be returned.
	vald = pow(0.0,3.14159);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = pow(-0.0,3.14159);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// If x is -1, and y is ±Inf, 1.0 shall be returned.
	if(pow(-1.0,__inf()) != 1.0) errCnt++;
	if(pow(-1.0,-__inf()) != 1.0) errCnt++;
	// For y < 0, if x is +Inf, +0 shall be returned.
	vald = pow(__inf(),-3.14519);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// For y > 0, if x is +Inf, +Inf shall be returned
	vald = pow(__inf(),3.14519);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	// For y an odd integer < 0, if x is -Inf, -0 shall be returned.
	vald = pow(-__inf(),-5);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	// For y < 0 and not an odd integer, if x is -Inf, +0 shall be returned.
	vald = pow(-__inf(),-3.14159);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// For y an odd integer > 0, if x is -Inf, -Inf shall be returned.
	if(!isinf(pow(-__inf(),5.0))) errCnt++;
	// For y > 0 and not an odd integer, if x is -Inf, +Inf shall be returned.
	if(!isinf(pow(-__inf(),3.14159))) errCnt++;
	// For |x| < 1, if y is +Inf, +0 shall be returned.
	vald = pow(0.99,__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = pow(-0.99,__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// If x is -1, and y is ±Inf, 1.0 shall be returned.
	if(pow(-1.0,__inf()) != 1.0) errCnt++;
	if(pow(-1.0,-__inf()) != 1.0) errCnt++;
	// For |x| > 1, if y is +Inf, +Inf shall be returned.
	if(!isinf(pow(3.14159,__inf()))) errCnt++;
	if(!isinf(pow(-3.14159,__inf()))) errCnt++;
	// For |x| < 1, if y is -Inf, +Inf shall be returned.
	if(!isinf(pow(0.99,-__inf()))) errCnt++;
	if(!isinf(pow(-0.99,-__inf()))) errCnt++;
	// If x is -1, and y is ±Inf, 1.0 shall be returned.
	if(pow(-1.0,__inf()) != 1.0) errCnt++;
	if(pow(-1.0,-__inf()) != 1.0) errCnt++;
	// For |x| > 1, if y is -Inf, +0 shall be returned.
	vald = pow(3.14159,-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = pow(-3.14159,-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	// For y < 0, if x is zero, a pole error may occur and pow(), powf(), and powl() shall return ±HUGE_VAL, ±HUGE_VALF, and ±HUGE_VALL, respectively
	vald = pow(0.0,-3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	// For finite values of x < 0, and finite non-integer values of y, a domain error shall occur and either a NaN (if representable), or an implementation-defined value shall be returned.
	if(!isnan(pow(-3.14159,3.14159) )) errCnt++;
	if(!isnan(pow(-3.14159,-3.14159)))  errCnt++;
	if(!fcntFloatInRange(powf(3.14159f,3.14159f),36.461952f)) errCnt++;
	valf = powf(__inff(),__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = powf(__inff(),-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	vald = powf(-__inff(),__inff());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	valf = powf(-__inff(),-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	// For any value of y (including NaN), if x is +1, 1.0 shall be returned.
	if(powf(1.0f,nanf(NULL)) != 1.0f) errCnt++;
	if(powf(1.0f,__inff()) != 1.0f) errCnt++;
	if(powf(1.0f,-__inff()) != 1.0f) errCnt++;
	// For any value of x (including NaN), if y is ±0, 1.0 shall be returned.
	if(powf(nanf(NULL),0.0f) != 1.0f) errCnt++;
	if(powf(nanf(NULL),-0.0f) != 1.0f) errCnt++;
	if(powf(__inff(),0.0f) != 1.0f) errCnt++;
	if(powf(__inff(),-0.0f) != 1.0f) errCnt++;
	if(powf(-__inff(),0.0f) != 1.0f) errCnt++;
	if(powf(-__inff(),-0.0f) != 1.0f) errCnt++;
	// If x or y is a NaN, a NaN shall be returned (unless specified elsewhere in this description).
	if(!isnan(powf(nanf(NULL),3.14159f))) errCnt++;
	if(!isnan(powf(3.14159f,nanf(NULL)))) errCnt++;
	if(!isnan(powf(nanf(NULL),nanf(NULL)))) errCnt++;
	// For any odd integer value of y > 0, if x is ±0, ±0 shall be returned.
	valf = powf(0.0f,5.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = powf(-0.0f,5.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	// For y > 0 and not an odd integer, if x is ±0, +0 shall be returned.
	valf = powf(0.0f,3.14159f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = powf(-0.0f,3.14159f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	// If x is -1, and y is ±Inf, 1.0 shall be returned.
	if(powf(-1.0f,__inff()) != 1.0f) errCnt++;
	if(powf(-1.0f,-__inff()) != 1.0f) errCnt++;
	// For y < 0, if x is +Inf, +0 shall be returned.
	valf = powf(__inff(),-3.14519f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	// For y > 0, if x is +Inf, +Inf shall be returned
	valf = powf(__inff(),3.14519f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	// For y an odd integer < 0, if x is -Inf, -0 shall be returned.
	valf = powf(-__inff(),-5.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	// For y < 0 and not an odd integer, if x is -Inf, +0 shall be returned.
	valf = powf(-__inff(),-3.14159f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	// For y an odd integer > 0, if x is -Inf, -Inf shall be returned.
	if(!isinf(powf(-__inff(),5.0f))) errCnt++;
	// For y > 0 and not an odd integer, if x is -Inf, +Inf shall be returned.
	if(!isinf(powf(-__inff(),3.14159f))) errCnt++;
	// For |x| < 1, if y is +Inf, +0 shall be returned.
	valf = powf(0.99f,__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = powf(-0.99f,__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	// If x is -1, and y is ±Inf, 1.0 shall be returned.
	if(powf(-1.0f,__inff()) != 1.0f) errCnt++;
	if(powf(-1.0f,-__inff()) != 1.0f) errCnt++;
	// For |x| > 1, if y is +Inf, +Inf shall be returned.
	if(!isinf(powf(3.14159f,__inff()))) errCnt++;
	if(!isinf(powf(-3.14159f,__inff()))) errCnt++;
	// For |x| < 1, if y is -Inf, +Inf shall be returned.
	if(!isinf(powf(0.99f,-__inff()))) errCnt++;
	if(!isinf(powf(-0.99f,-__inff()))) errCnt++;
	// If x is -1, and y is ±Inf, 1.0 shall be returned.
	if(powf(-1.0f,__inff()) != 1.0f) errCnt++;
	if(powf(-1.0f,-__inff()) != 1.0f) errCnt++;
	// For |x| > 1, if y is -Inf, +0 shall be returned.
	valf = powf(3.14159f,-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = powf(-3.14159f,-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	// For y < 0, if x is zero, a pole error may occur and pow(), powf(), and powl() shall return ±HUGE_VAL, ±HUGE_VALF, and ±HUGE_VALL, respectively
	valf = powf(0.0f,-3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	// For finite values of x < 0, and finite non-integer values of y, a domain error shall occur and either a NaN (if representable), or an implementation-defined value shall be returned.
	if(!isnan(powf(-3.14159f,3.14159f))) errCnt++;
	if(!isnan(powf(-3.14159f,-3.14159f))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE45 errCnt = %u\n",errCnt);
#endif

	if(!isnan((2.2*nan(NULL))/(2.2*nan(NULL)))) errCnt++;
	// Test remainder, remainderf
	if(!fcntDoubleInRange(remainder(5.1,3.0),-0.9)) errCnt++;
	// If x or y is NaN, a NaN shall be returned.
	if(!isnan(remainder(nan(NULL),nan(NULL)))) errCnt++;
	if(!isnan(remainder(nan(NULL),5.1))) errCnt++;
	if(!isnan(remainder(5.1,nan(NULL)))) errCnt++;
	if(!isnan(remainder(5.1,nan(NULL)))) errCnt++;
	// If x is infinite or y is 0 and the other is non-NaN, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(remainder(__inf(),3.0))) errCnt++;
	if(!isnan(remainder(-__inf(),3.0))) errCnt++;
	if(!isnan(remainder(5.0,0.0))) errCnt++;
	if(!fcntFloatInRange(remainderf(5.1f,3.0f),-0.9f)) errCnt++;
	// If x or y is NaN, a NaN shall be returned.
	if(!isnan(remainderf(nanf(NULL),nanf(NULL)))) errCnt++;
	if(!isnan(remainderf(nanf(NULL),5.1f))) errCnt++;
	if(!isnan(remainderf(5.1f,nanf(NULL)))) errCnt++;
	if(!isnan(remainderf(5.1f,nanf(NULL)))) errCnt++;
	// If x is infinite or y is 0 and the other is non-NaN, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(remainderf(__inff(),3.0f))) errCnt++;
	if(!isnan(remainderf(-__inff(),3.0f))) errCnt++;
	if(!isnan(remainderf(5.0f,0.0f))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE46 errCnt = %u\n",errCnt);
#endif

	// Test remquo, remquof
	int quotient;
	double remainder;
	float remainderf;

	remainder = remquo(12.5,2.2,&quotient);
	if(!fcntDoubleInRange(remainder,-0.7) || quotient != 6) errCnt++;
	// If x or y is NaN, a NaN shall be returned.
	if(!isnan(remquo(nan(NULL),2.2,&quotient))) errCnt++;
	if(!isnan(remquo(12.5,nan(NULL),&quotient))) errCnt++;
	if(!isnan(remquo(nan(NULL),nan(NULL),&quotient))) errCnt++;
	// If x is ±Inf or y is zero and the other argument is non-NaN, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(remquo(__inf(),2.2,&quotient))) errCnt++;
	if(!isnan(remquo(-__inf(),2.2,&quotient))) errCnt++;
	if(!isnan(remquo(12.5,0.0,&quotient))) errCnt++;
	remainderf = remquof(12.5f,2.2f,&quotient);
	if(!fcntFloatInRange(remainderf,-0.7f) || quotient != 6) errCnt++;
	// If x or y is NaN, a NaN shall be returned.
	if(!isnan(remquof(nanf(NULL),2.2f,&quotient))) errCnt++;
	if(!isnan(remquof(12.5f,nanf(NULL),&quotient))) errCnt++;
	if(!isnan(remquof(nanf(NULL),nanf(NULL),&quotient))) errCnt++;
	// If x is ±Inf or y is zero and the other argument is non-NaN, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(remquof(__inff(),2.2f,&quotient))) errCnt++;
	if(!isnan(remquof(-__inff(),2.2f,&quotient))) errCnt++;
	if(!isnan(remquof(12.5f,0.0f,&quotient))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE47 errCnt = %u\n",errCnt);
#endif

	// Test rint, rintf
	if(!fcntDoubleInRange(rint(4.159),4.0)) errCnt++;
	if(!fcntDoubleInRange(rint(4.95),5.0)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(rint(nan(NULL)))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	vald = rint(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = rint(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = rint(0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = rint(-0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	if(!fcntFloatInRange(rintf(4.159f),4.0f)) errCnt++;
	if(!fcntFloatInRange(rintf(4.95f),5.0f)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(rintf(nanf(NULL)))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	valf = rintf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = rintf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = rintf(0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = rintf(-0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE48 errCnt = %u\n",errCnt);
#endif

	// Test round, roundf
	if(!fcntDoubleInRange(round(5.1),5.0)) errCnt++;
	if(!fcntDoubleInRange(round(5.6),6.0)) errCnt++;
	if(!fcntDoubleInRange(round(-5.6),-6.0)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(round(nan(NULL)))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	vald = round(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = round(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = round(0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = round(-0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	if(!fcntFloatInRange(roundf(5.1f),5.0f)) errCnt++;
	if(!fcntFloatInRange(roundf(5.6f),6.0f)) errCnt++;
	if(!fcntFloatInRange(roundf(-5.6f),-6.0f)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(roundf(nanf(NULL)))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	valf = roundf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = roundf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = roundf(0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = roundf(-0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE49 errCnt = %u\n",errCnt);
#endif

	// Test scalbn, scalbnf
	if(!fcntDoubleInRange(scalbn(1971.1969,5L),63078.3008)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(scalbn(nan(NULL),5L))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	vald = scalbn(0.0,5L);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = scalbn(-0.0,5L);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = scalbn(__inf(),5L);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = scalbn(-__inf(),5L);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If n is 0, x shall be returned. 
	if(scalbn(1971.1969,0L) != 1971.1969) errCnt++;
	if(!fcntFloatInRange(scalbnf(1971.1969f,5L),63078.3008f)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(scalbnf(nanf(NULL),5L))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	valf = scalbnf(0.0f,5L);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = scalbnf(-0.0f,5L);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = scalbnf(__inff(),5L);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = scalbnf(-__inff(),5L);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	// If n is 0, x shall be returned. 
	if(scalbnf(1971.1969f,0L) != 1971.1969f) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE50 errCnt = %u\n",errCnt);
#endif

	// Test signbit
	if(signbit(4.0) != 0) errCnt++;
	if(signbit(4.0f) != 0) errCnt++;
	if(signbit(-2.0) == 0) errCnt++;
	if(signbit(-2.0f) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE51 errCnt = %u\n",errCnt);
#endif

	// Test sin, sinf, sinh, sinhf
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntDoubleInRange(sin(0.5),0.4794)) errCnt++;
	if(!isnan(sin(nan(NULL)))) errCnt++;
	if(!isnan(sin(__inf()))) errCnt++;
	if(!isnan(sin(-__inf()))) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and a NaN shall be returned.
	if(!fcntFloatInRange(sinf(0.5f),0.4794f)) errCnt++;
	if(!isnan(sinf(nanf(NULL)))) errCnt++;
	if(!isnan(sinf(__inff()))) errCnt++;
	if(!isnan(sinf(-__inff()))) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntDoubleInRange(sinh(1.256),1.6132)) errCnt++;
	if(!isnan(sinh(nan(NULL)))) errCnt++;
	vald = sinh(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = sinh(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±0 or ±Inf, x shall be returned.
	if(!fcntFloatInRange(sinhf(1.256f),1.6132f)) errCnt++;
	if(!isnan(sinhf(nanf(NULL)))) errCnt++;
	valf = sinhf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = sinhf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE52 errCnt = %u\n",errCnt);
#endif

	// Test sqrt, sqrtf
	if(!fcntDoubleInRange(sqrt(0.5),0.7071)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(sqrt(nan(NULL)))) errCnt++;
	// If x is ±0 or +Inf, x shall be returned.
	vald = sqrt(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	// If x is -Inf, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(sqrt(-__inf()))) errCnt++;
	// For finite values of x < -0, a domain error shall occur, and  either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!isnan(sqrt(-4.0))) errCnt++;
	if(!fcntFloatInRange(sqrtf(0.5f),0.7071f)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(sqrtf(nanf(NULL)))) errCnt++;
	// If x is ±0 or +Inf, x shall be returned.
	valf = sqrtf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	// If x is -Inf, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(sqrtf(-__inff()))) errCnt++;
	// For finite values of x < -0, a domain error shall occur, and  either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!isnan(sqrtf(-4.0f))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE53 errCnt = %u\n",errCnt);
#endif

	// Test tan, tanf, tanh, tanhf
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntDoubleInRange(tan(0.5),0.5463)) errCnt++;
	if(!isnan(tan(nan(NULL)))) errCnt++;
	if(!isnan(tan(__inf()))) errCnt++;
	if(!isnan(tan(-__inf()))) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, a domain error shall occur, and either a NaN (if supported), or an implementation-defined value shall be returned.
	if(!fcntFloatInRange(tanf(0.5f),0.5463f)) errCnt++;
	if(!isnan(tanf(nanf(NULL)))) errCnt++;
	if(!isnan(tanf(__inff()))) errCnt++;
	if(!isnan(tanf(-__inff()))) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, ±1 shall be returned.
	if(!fcntDoubleInRange(tanh(1.256),0.8499)) errCnt++;
	if(!isnan(tanh(nan(NULL)))) errCnt++;
	if(!fcntDoubleInRange(tanh(__inf()),1.0)) errCnt++;
	if(!fcntDoubleInRange(tanh(-__inf()),-1.0)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	// If x is ±Inf, ±1 shall be returned.
	if(!fcntFloatInRange(tanhf(1.256f),0.8499f)) errCnt++;
	if(!isnan(tanhf(nanf(NULL)))) errCnt++;
	if(!fcntFloatInRange(tanhf(__inff()),1.0f)) errCnt++;
	if(!fcntFloatInRange(tanhf(-__inff()),-1.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE54 errCnt = %u\n",errCnt);
#endif

	// Test tgamma, tgammaf
	if(!fcntDoubleInRange(tgamma(0.5),1.772454)) errCnt++;
	// If x is ±0, tgamma(), tgammaf(), and tgammal() shall return ±HUGE_VAL, ±HUGE_VALF, and ±HUGE_VALL, respectively.
	// On systems that support the IEC 60559 Floating-Point option, a pole error shall occur; otherwise, a pole error may occur.
	vald = tgamma(0.0);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = tgamma(-0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(tgamma(nan(NULL)))) errCnt++;
	// If x is +Inf, x shall be returned.
	vald = tgamma(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	// If x is -Inf, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(tgamma(-__inf()))) errCnt++;
	if(!fcntFloatInRange(tgammaf(0.5f),1.772454f)) errCnt++;
	// If x is ±0, tgamma(), tgammaf(), and tgammal() shall return ±HUGE_VAL, ±HUGE_VALF, and ±HUGE_VALL, respectively.
	// On systems that support the IEC 60559 Floating-Point option, a pole error shall occur; otherwise, a pole error may occur.
	valf = tgammaf(0.0f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = tgammaf(-0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(tgammaf(nanf(NULL)))) errCnt++;
	// If x is +Inf, x shall be returned.
	valf = tgammaf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	// If x is -Inf, a domain error shall occur, and a NaN shall be returned.
	if(!isnan(tgammaf(-__inff()))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE55 errCnt = %u\n",errCnt);
#endif

	// Test trunc, trunf
	if(!fcntDoubleInRange(trunc(1971.1969),1971.0)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(trunc(nan(NULL)))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	vald = trunc(0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = trunc(-0.0);
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = trunc(__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = trunc(-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!fcntFloatInRange(truncf(1971.1969f),1971.0f)) errCnt++;
	// If x is NaN, a NaN shall be returned.
	if(!isnan(truncf(nanf(NULL)))) errCnt++;
	// If x is ±0 or ±Inf, x shall be returned.
	valf = truncf(0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = truncf(-0.0f);
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = truncf(__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = truncf(-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE56 errCnt = %u\n",errCnt);
#endif

	// Test asprintf
	char *ret;

	strcpy(test_strings,TEST_STRING_1);
	strcat(test_strings,TEST_STRING_2);

	if(asprintf(&ret,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(strlen(TEST_STRING_1) + strlen(TEST_STRING_2)))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(ret,test_strings,strlen(TEST_STRING_1) + strlen(TEST_STRING_2)))
			errCnt++;

		free(ret);
	}

#if OPTION_TRACE_MATH
	printf("TRACE57 errCnt = %u\n",errCnt);
#endif

	// Test fprintf, fscanf
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(fprintf(file_ptr,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(strlen(TEST_STRING_1) + strlen(TEST_STRING_2)))
		{
			errCnt++;
		}

		fclose(file_ptr);

		if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
		{
			errCnt++;
		}
		else
		{
			memset(buffer1,0,BUFFER_SIZE);

			if(fscanf(file_ptr,"%s",&buffer1) != 1)
			{
				errCnt++;
			}
			else
			{
				if(strncmp(TEST_STRING_1_2,buffer1,2)) // first word
				{
					errCnt++;
				}
			}

			fclose(file_ptr);
		}
	}

#if OPTION_TRACE_MATH
	printf("TRACE58 errCnt = %u\n",errCnt);
#endif

	// Test scanf, vscanf
	int nb1,nb2;

	printf("Please type two numbers separated by a space:\n");

	if(scanf("%d %d",&nb1,&nb2) != 2)
	{
		errCnt++;
		printf("scanf error\n");
	}
	else
	{
		printf("%d + %d = %d\n",nb1,nb2,nb1 + nb2);
	}

	printf("Please type two other numbers separated by a space:\n");

	if(math_vscanf_test("%d %d",&nb1,&nb2) != 2)
	{
		errCnt++;
		printf("vscanf error\n");
	}
	else
	{
		printf("%d + %d = %d\n",nb1,nb2,nb1 + nb2);
	}

#if OPTION_TRACE_MATH
	printf("TRACE59 errCnt = %u\n",errCnt);
#endif

	// Test vprintf
	printf("Testing vprintf:");
	if(math_vprintf_test("%s%s\n",TEST_STRING_1,TEST_STRING_2) != (int)strlen(TEST_STRING_1_2) + 1) // + 1 for \n
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE60 errCnt = %u\n",errCnt);
#endif

	// Test snprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(snprintf(buffer1,strlen(TEST_STRING_1) + 1,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(strlen(TEST_STRING_1) + strlen(TEST_STRING_2))) // + 1 to include the ending '\0'
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1)))
			errCnt++;
	}

#if OPTION_TRACE_MATH
	printf("TRACE61 errCnt = %u\n",errCnt);
#endif

	// Test sprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(sprintf(buffer1,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(strlen(TEST_STRING_1) + strlen(TEST_STRING_2)))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
			errCnt++;
	}

#if OPTION_TRACE_MATH
	printf("TRACE62 errCnt = %u\n",errCnt);
#endif

	// Test sprintf, sscanf
	static int nbr1,nbr2;

	memset(buffer1,0,BUFFER_SIZE);

	if(sprintf(buffer1,"%d %d",NBR1,NBR2) < 0)
	{
		errCnt++;
	}
	else
	{
		if(sscanf(buffer1,"%d %d",&nbr1,&nbr2) != 2)
		{
			errCnt++;
		}
		else
		{
			if(nbr1 != NBR1 || nbr2 != NBR2)
			{
				errCnt++;
			}
		}
	}

#if OPTION_TRACE_MATH
	printf("TRACE63 errCnt = %u\n",errCnt);
#endif

	// Test vasprintf
	char *vret;
	int len;

	if((len = math_vasprintf_test(&vret,"%s%s",TEST_STRING_1,TEST_STRING_2)) != (int)strlen(TEST_STRING_1_2))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(vret,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
			errCnt++;

		free(vret);
	}

#if OPTION_TRACE_MATH
	printf("TRACE64 errCnt = %u\n",errCnt);
#endif

	// Test vfprintf
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(math_vfprintf_test(file_ptr,"%s%s",TEST_STRING_1,TEST_STRING_2) < 0)
		{
			errCnt++;
		}
		else
		{
			fclose(file_ptr);
			file_ptr = NULL;

			if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
			{
				errCnt++;
			}
			else
			{
				memset(buffer1,0,BUFFER_SIZE);

				if(fread(buffer1,strlen(TEST_STRING_1_2),1,file_ptr) != 1)
				{
					errCnt++;
				}
				else
				{
					if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
					{
						errCnt++;
					}
				}

				fclose(file_ptr);
			}
		}
	}

#if OPTION_TRACE_MATH
	printf("TRACE65 errCnt = %u\n",errCnt);
#endif

	// Test vfscanf
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(fwrite(LOVE_STRING,strlen(LOVE_STRING),1,file_ptr) != 1)
		{
			errCnt++;
		}
		else
		{
			fclose(file_ptr);
			file_ptr = NULL;

			if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
			{
				errCnt++;
			}
			else
			{
				memset(buffer1,0,BUFFER_SIZE);

				if(math_vfscanf_test(file_ptr,"%s",&buffer1) == EOF)
				{
					errCnt++;
				}
				else
				{
					if(strncmp(buffer1,LOVE_STRING,3)) // first word
					{
						errCnt++;
					}
				}

				fclose(file_ptr);
				file_ptr = NULL;
			}
		}
	}

#if OPTION_TRACE_MATH
	printf("TRACE66 errCnt = %u\n",errCnt);
#endif

	// Test vsnprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(math_vsnprintf_test(buffer1,BUFFER_SIZE,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)strlen(TEST_STRING_1_2))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
			errCnt++;
	}

#if OPTION_TRACE_MATH
	printf("TRACE67 errCnt = %u\n",errCnt);
#endif

	// Test vsprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(math_vsprintf_test(buffer1,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)strlen(TEST_STRING_1_2))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
			errCnt++;
	}

#if OPTION_TRACE_MATH
	printf("TRACE68 errCnt = %u\n",errCnt);
#endif

	// Test vsscanf
	memset(buffer1,0,BUFFER_SIZE);

	if(sprintf(buffer1,"%d %d",NBR1,NBR2) < 0)
	{
		errCnt++;
	}
	else
	{
		if(math_vsscanf_test(buffer1,"%d %d",&nbr1,&nbr2) != 2)
		{
			errCnt++;
		}
		else
		{
			if(nbr1 != NBR1 || nbr2 != NBR2)
				errCnt++;
		}
	}

#if OPTION_TRACE_MATH
	printf("TRACE69 errCnt = %u\n",errCnt);
#endif

	// Test atof
	if(!fcntDoubleInRange(atof("3.14159"),3.14159)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE70 errCnt = %u\n",errCnt);
#endif

	// Test strtod
	if(!fcntDoubleInRange(strtod("3.14159",NULL),3.14159)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE71 errCnt = %u\n",errCnt);
#endif

	// Test strtof
	if(!fcntFloatInRange(strtof("3.14159",NULL),3.14159f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE72 errCnt = %u\n",errCnt);
#endif

	// Test difftime
	time_t begin = time( NULL );    
    sleep(2);   // Is a POSIX function - defined in <unistd.h>
    time_t end = time( NULL);
    unsigned long seconds = (unsigned long) difftime(end,begin);
	if(seconds != 2L) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE73 errCnt = %u\n",errCnt);
#endif

	// Test strlcpy, strlcat
	strlcpy(buffer1,TEST_STRING_1,BUFFER_SIZE);
	strlcat(buffer1,TEST_STRING_2,BUFFER_SIZE);

	if(strcmp(buffer1,TEST_STRING_1_2))
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE74 errCnt = %u\n",errCnt);
#endif

	// Test udivsi3
	if(__udivsi3(4294967295UL,16777216UL) != 255UL)
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE75 errCnt = %u\n",errCnt);
#endif

	// Test divsi3
	if(__divsi3(-16777216L,439L) != -38216L)
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE76 errCnt = %u\n",errCnt);
#endif

	// Test mulsi3
	if(__mulsi3(-16777216L,10L) != -167772160L)
		errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE77 errCnt = %u\n",errCnt);
#endif

	// Test floatunsisf
	if(!fcntFloatInRange(__floatunsisf(4294967295UL),4294967295.0f)) errCnt++;
	if(!fcntFloatInRange(__floatunsisf(1965UL),1965.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE78 errCnt = %u\n",errCnt);
#endif

	// Test floatunsidf
	if(!fcntDoubleInRange(__floatunsidf(4294967295UL),4294967295.0)) errCnt++;
	if(!fcntDoubleInRange(__floatunsidf(1965UL),1965.0)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE79 errCnt = %u\n",errCnt);
#endif

	// Test adddf3, addsf3
	if(!fcntDoubleInRange(__adddf3(3.14159,2.71828),5.85987)) errCnt++;
	if(!isnan(__adddf3(nan(NULL),3.14159))) errCnt++;
	if(!isnan(__adddf3(3.14159,nan(NULL)))) errCnt++;
	vald = __adddf3(__inf(),__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __adddf3(-__inf(),-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!isnan(__adddf3(__inf(),-__inf()))) errCnt++;
	if(!isnan(__adddf3(-__inf(),__inf()))) errCnt++;
	vald = __adddf3(__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __adddf3(-__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __adddf3(3.14159,__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __adddf3(3.14159,-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!fcntFloatInRange(__addsf3(3.14159f,2.71828f),5.85987f)) errCnt++;
	if(!isnan(__addsf3(nan(NULL),3.14159f))) errCnt++;
	if(!isnan(__addsf3(3.14159f,nan(NULL)))) errCnt++;
	valf = __addsf3(__inff(),__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __addsf3(-__inff(),-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	if(!isnan(__addsf3(__inff(),-__inff()))) errCnt++;
	if(!isnan(__addsf3(-__inff(),__inff()))) errCnt++;
	valf = __addsf3(__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __addsf3(-__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __addsf3(3.14159f,__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __addsf3(3.14159f,-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE80 errCnt = %u\n",errCnt);
#endif

	// Test divdf3, divsf3
	if(!fcntDoubleInRange(__divdf3(3.14159,2.71828),1.15572)) errCnt++;
	if(!isnan(__divdf3(nan(NULL),3.14159))) errCnt++;
	if(!isnan(__divdf3(3.14159,nan(NULL)))) errCnt++;
	vald = __divdf3(3.14159,0.0);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __divdf3(3.14159,-0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-3.14159,0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-3.14159,-0.0);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __divdf3(__inf(),0.0);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __divdf3(-__inf(),0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __divdf3(__inf(),-0.0);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-__inf(),-0.0);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	if(!isnan(__divdf3(0.0,0.0))) errCnt++;
	if(!isnan(__divdf3(-0.0,0.0))) errCnt++;
	if(!isnan(__divdf3(0.0,-0.0))) errCnt++;
	if(!isnan(__divdf3(-0.0,-0.0))) errCnt++;
	if(!isnan(__divdf3(__inf(),__inf()))) errCnt++;
	if(!isnan(__divdf3(-__inf(),-__inf()))) errCnt++;
	if(!isnan(__divdf3(__inf(),-__inf()))) errCnt++;
	if(!isnan(__divdf3(-__inf(),__inf()))) errCnt++;
	vald = __divdf3(__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __divdf3(__inf(),-3.14159);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-__inf(),-3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __divdf3(3.14159,__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = __divdf3(3.14159,-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-3.14159,__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-3.14159,-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = __divdf3(0.0,__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	vald = __divdf3(0.0,-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-0.0,__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) == 0) errCnt++;
	vald = __divdf3(-0.0,-__inf());
	if(fpclassify(vald) != FP_ZERO || signbit(vald) != 0) errCnt++;
	if(!fcntFloatInRange(__divsf3(3.14159f,2.71828f),1.15572f)) errCnt++;
	if(!isnan(__divsf3(nanf(NULL),3.14159f))) errCnt++;
	if(!isnan(__divsf3(3.14159f,nanf(NULL)))) errCnt++;
	valf = __divsf3(3.14159f,0.0f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __divsf3(3.14159f,-0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-3.14159f,0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-3.14159f,-0.0f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __divsf3(__inff(),0.0f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __divsf3(-__inff(),0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __divsf3(__inff(),-0.0f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-__inff(),-0.0f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	if(!isnan(__divsf3(0.0f,0.0f))) errCnt++;
	if(!isnan(__divsf3(-0.0f,0.0f))) errCnt++;
	if(!isnan(__divsf3(0.0f,-0.0f))) errCnt++;
	if(!isnan(__divsf3(-0.0f,-0.0f))) errCnt++;
	if(!isnan(__divsf3(__inff(),__inff()))) errCnt++;
	if(!isnan(__divsf3(-__inff(),-__inff()))) errCnt++;
	if(!isnan(__divsf3(__inff(),-__inff()))) errCnt++;
	if(!isnan(__divsf3(-__inff(),__inff()))) errCnt++;
	valf = __divsf3(__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __divsf3(__inff(),-3.14159f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-__inff(),-3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __divsf3(3.14159f,__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = __divsf3(3.14159f,-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-3.14159f,__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-3.14159f,-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = __divsf3(0.0f,__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;
	valf = __divsf3(0.0f,-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-0.0f,__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) == 0) errCnt++;
	valf = __divsf3(-0.0f,-__inff());
	if(fpclassify(valf) != FP_ZERO || signbit(valf) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE81 errCnt = %u\n",errCnt);
#endif

	// Test eqdf2, eqsf2
	if(__eqdf2(3.14159,2.71828) == 0) errCnt++;
	if(__eqdf2(2.71828,3.14159) == 0) errCnt++;
	if(__eqdf2(3.14159,3.14159) != 0) errCnt++;
	// __eqdf2 returns zero if neither argument is NaN, and a and b are equal. 
	if(__eqdf2(nan(NULL),3.14159) == 0) errCnt++;
	if(__eqdf2(3.14159,nan(NULL)) == 0) errCnt++;
	if(__eqdf2(nan(NULL),nan(NULL)) == 0) errCnt++;
	if(__eqdf2(__inf(),__inf()) != 0) errCnt++;
	if(__eqdf2(-__inf(),-__inf()) != 0) errCnt++;
	if(__eqdf2(__inf(),-__inf()) == 0) errCnt++;
	if(__eqdf2(-__inf(),__inf()) == 0) errCnt++;
	if(__eqsf2(3.14159f,2.71828f) == 0) errCnt++;
	if(__eqsf2(2.71828f,3.14159f) == 0) errCnt++;
	if(__eqsf2(3.14159f,3.14159f) != 0) errCnt++;
	// __eqsf2 returns zero if neither argument is NaN, and a and b are equal. 
	if(__eqsf2(nanf(NULL),3.14159f) == 0) errCnt++;
	if(__eqsf2(3.14159f,nanf(NULL)) == 0) errCnt++;
	if(__eqsf2(nanf(NULL),nanf(NULL)) == 0) errCnt++;
	if(__eqsf2(__inff(),__inff()) != 0) errCnt++;
	if(__eqsf2(-__inff(),-__inff()) != 0) errCnt++;
	if(__eqsf2(__inff(),-__inff()) == 0) errCnt++;
	if(__eqsf2(-__inff(),__inff()) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE82 errCnt = %u\n",errCnt);
#endif

	// Test extendsfdf2
	// __extendsfdf2 extends a to the wider mode of their return type. 
	double ex;
	if(!fcntDoubleInRange(__extendsfdf2(2.71828f),2.71828)) errCnt++;
	if(!isnan(__extendsfdf2(nanf(NULL)))) errCnt++;
	vald = __extendsfdf2(__inff());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __extendsfdf2(-__inff());
	if(!isinf(vald) || signbit(vald) != 1) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE83 errCnt = %u\n",errCnt);
#endif

	// Test fixdfsi, fixsfsi
	// __fixdfsi converts a to a signed integer, rounding toward zero.
	if(__fixdfsi(-3.14159) != -3L) errCnt++;
	if(__fixdfsi(nan(NULL)) != 0L) errCnt++;
	if(__fixdfsi(__inf()) != 0L) errCnt++;
	if(__fixdfsi(-__inf()) != 0L) errCnt++;
	// __fixsfsi converts a to a signed integer, rounding toward zero.
	if(__fixsfsi(-3.14159f) != -3L) errCnt++;
	if(__fixsfsi(nanf(NULL)) != 0L) errCnt++;
	if(__fixsfsi(__inff()) != 0L) errCnt++;
	if(__fixsfsi(-__inff()) != 0L) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE84 errCnt = %u\n",errCnt);
#endif

	// Test fixunsdfsi, fixunssfsi
	// __fixdfsi converts a to a signed integer, rounding toward zero.
	if(__fixunsdfsi(-3.14159) != 0) errCnt++;
	if(__fixunsdfsi(3.14159) != 3) errCnt++;
	if(__fixunsdfsi(nan(NULL)) != 0) errCnt++;
	if(__fixunsdfsi(__inf()) != 0) errCnt++;
	if(__fixunsdfsi(-__inf()) != 0) errCnt++;
	// __fixunssfsi converts a to a signed integer, rounding toward zero.
	if(__fixunssfsi(-3.14159f) != 0) errCnt++;
	if(__fixunssfsi(3.14159f) != 3) errCnt++;
	if(__fixunssfsi(nanf(NULL)) != 0) errCnt++;
	if(__fixunssfsi(__inff()) != 0) errCnt++;
	if(__fixunssfsi(-__inff()) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE85 errCnt = %u\n",errCnt);
#endif

	// Test floatsidf, floatsisf
	if(__floatsidf(-4L) != -4.0) errCnt++;
	if(__floatsisf(-4L) != -4.0f) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE86 errCnt = %u\n",errCnt);
#endif

	// Test gedf2, gesf2
	// __gedf2 returns a value greater than or equal to zero if neither argument is NaN, and a is greater than or equal to b. 
	if(__gedf2(3.14159,2.71828) < 0) errCnt++;
	if(__gedf2(3.14159,3.14159) < 0) errCnt++;
	if(__gedf2(-3.14159,-2.71828) >= 0) errCnt++;
	if(__gedf2(nan(NULL),nan(NULL)) >= 0) errCnt++;
	if(__gedf2(nan(NULL),3.14159) >= 0) errCnt++;
	if(__gedf2(3.14159,nan(NULL)) >= 0) errCnt++;
	if(__gedf2(__inf(),__inf()) < 0) errCnt++;
	if(__gedf2(-__inf(),-__inf()) < 0) errCnt++;
	if(__gedf2(__inf(),-__inf()) < 0) errCnt++;
	if(__gedf2(-__inf(),__inf()) >= 0) errCnt++;
	if(__gedf2(__inf(),3.14159) < 0) errCnt++;
	if(__gedf2(3.14159,__inf()) >= 0) errCnt++;
	if(__gedf2(-__inf(),3.14159) >= 0) errCnt++;
	if(__gedf2(3.14159,-__inf()) < 0) errCnt++;
	// __gesf2 returns a value greater than or equal to zero if neither argument is NaN, and a is greater than or equal to b. 
	if(__gesf2(3.14159f,2.71828f) < 0) errCnt++;
	if(__gesf2(3.14159f,3.14159f) < 0) errCnt++;
	if(__gesf2(-3.14159f,-2.71828f) >= 0) errCnt++;
	if(__gesf2(nanf(NULL),nanf(NULL)) >= 0) errCnt++;
	if(__gesf2(nanf(NULL),3.14159) >= 0) errCnt++;
	if(__gesf2(3.14159,nanf(NULL)) >= 0) errCnt++;
	if(__gesf2(__inff(),__inff()) < 0) errCnt++;
	if(__gesf2(-__inff(),-__inff()) < 0) errCnt++;
	if(__gesf2(__inff(),-__inff()) < 0) errCnt++;
	if(__gesf2(-__inff(),__inff()) >= 0) errCnt++;
	if(__gesf2(__inff(),3.14159f) < 0) errCnt++;
	if(__gesf2(3.14159f,__inff()) >= 0) errCnt++;
	if(__gesf2(-__inff(),3.14159f) >= 0) errCnt++;
	if(__gesf2(3.14159f,-__inff()) < 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE87 errCnt = %u\n",errCnt);
#endif

	// Test gtdf2, gtsf2
	// gtdf2 returns a value greater than zero if neither argument is NaN, and a is strictly greater than b. 
	if(__gtdf2(3.14159,2.71828) <= 0) errCnt++;
	if(__gtdf2(3.14159,3.14159) > 0) errCnt++;
	if(__gtdf2(-3.14159,-2.71828) > 0) errCnt++;
	if(__gtdf2(nan(NULL),nan(NULL)) > 0) errCnt++;
	if(__gtdf2(nan(NULL),3.14159) > 0) errCnt++;
	if(__gtdf2(3.14159,nan(NULL)) > 0) errCnt++;
	if(__gtdf2(__inf(),__inf()) > 0) errCnt++;
	if(__gtdf2(-__inf(),-__inf()) > 0) errCnt++;
	if(__gtdf2(__inf(),-__inf()) <= 0) errCnt++;
	if(__gtdf2(-__inf(),__inf()) > 0) errCnt++;
	if(__gtdf2(__inf(),3.14159) <= 0) errCnt++;
	if(__gtdf2(3.14159,__inf()) > 0) errCnt++;
	if(__gtdf2(-__inf(),3.14159) > 0) errCnt++;
	if(__gtdf2(3.14159,-__inf()) <= 0) errCnt++;
	// gtsf2 returns a value greater than zero if neither argument is NaN, and a is strictly greater than b. 
	if(__gtsf2(3.14159f,2.71828f) <= 0) errCnt++;
	if(__gtsf2(3.14159f,3.14159f) > 0) errCnt++;
	if(__gtsf2(-3.14159f,-2.71828f) > 0) errCnt++;
	if(__gtsf2(nanf(NULL),nanf(NULL)) > 0) errCnt++;
	if(__gtsf2(nanf(NULL),3.14159f) > 0) errCnt++;
	if(__gtsf2(3.14159f,nanf(NULL)) > 0) errCnt++;
	if(__gtsf2(__inff(),__inff()) > 0) errCnt++;
	if(__gtsf2(-__inff(),-__inff()) > 0) errCnt++;
	if(__gtsf2(__inff(),-__inff()) <= 0) errCnt++;
	if(__gtsf2(-__inff(),__inff()) > 0) errCnt++;
	if(__gtsf2(__inff(),3.14159f) <= 0) errCnt++;
	if(__gtsf2(3.14159f,__inff()) > 0) errCnt++;
	if(__gtsf2(-__inff(),3.14159f) > 0) errCnt++;
	if(__gtsf2(3.14159f,-__inff()) <= 0) errCnt++;
	
#if OPTION_TRACE_MATH
	printf("TRACE88 errCnt = %u\n",errCnt);
#endif

	// Test ledf2, lesf2
	// __ledf2 returns a value less than or equal to zero if neither argument is NaN, and a is less than or equal to b. 
	if(__ledf2(2.71828,3.14159) > 0) errCnt++;
	if(__ledf2(3.14159,3.14159) > 0) errCnt++;
	if(__ledf2(-2.71828,-3.14159) <= 0) errCnt++;
	if(__ledf2(nan(NULL),nan(NULL)) <= 0) errCnt++;
	if(__ledf2(nan(NULL),3.14159) <= 0) errCnt++;
	if(__ledf2(3.14159,nan(NULL)) <= 0) errCnt++;
	if(__ledf2(__inf(),__inf()) > 0) errCnt++;
	if(__ledf2(-__inf(),-__inf()) > 0) errCnt++;
	if(__ledf2(__inf(),-__inf()) <= 0) errCnt++;
	if(__ledf2(-__inf(),__inf()) > 0) errCnt++;
	if(__ledf2(__inf(),3.14159) <= 0) errCnt++;
	if(__ledf2(3.14159,__inf()) > 0) errCnt++;
	if(__ledf2(-__inf(),3.14159) > 0) errCnt++;
	if(__ledf2(3.14159,-__inf()) <= 0) errCnt++;
	// __lesf2 returns a value less than or equal to zero if neither argument is NaN, and a is less than or equal to b. 
	if(__lesf2(2.71828f,3.14159f) > 0) errCnt++;
	if(__lesf2(3.14159f,3.14159f) > 0) errCnt++;
	if(__lesf2(-2.71828f,-3.14159f) <= 0) errCnt++;
	if(__lesf2(nanf(NULL),nanf(NULL)) <= 0) errCnt++;
	if(__lesf2(nanf(NULL),3.14159f) <= 0) errCnt++;
	if(__lesf2(3.14159f,nanf(NULL)) <= 0) errCnt++;
	if(__lesf2(__inff(),__inff()) > 0) errCnt++;
	if(__lesf2(-__inff(),-__inff()) > 0) errCnt++;
	if(__lesf2(__inff(),-__inff()) <= 0) errCnt++;
	if(__lesf2(-__inff(),__inff()) > 0) errCnt++;
	if(__lesf2(__inff(),3.14159f) <= 0) errCnt++;
	if(__lesf2(3.14159f,__inff()) > 0) errCnt++;
	if(__lesf2(-__inff(),3.14159f) > 0) errCnt++;
	if(__lesf2(3.14159f,-__inff()) <= 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE89 errCnt = %u\n",errCnt);
#endif

	// Test ltdf2, ltsf2
	// __ltdf2 returns a value less than zero if neither argument is NaN, and a is strictly less than b. 
	if(__ltdf2(2.71828,3.14159) >= 0) errCnt++;
	if(__ltdf2(3.14159,3.14159) < 0) errCnt++;
	if(__ltdf2(-2.71828,-3.14159) < 0) errCnt++;
	if(__ltdf2(nan(NULL),nan(NULL)) < 0) errCnt++;
	if(__ltdf2(nan(NULL),3.14159) < 0) errCnt++;
	if(__ltdf2(3.14159,nan(NULL)) < 0) errCnt++;
	if(__ltdf2(__inf(),__inf()) < 0) errCnt++;
	if(__ltdf2(-__inf(),-__inf()) < 0) errCnt++;
	if(__ltdf2(__inf(),-__inf()) < 0) errCnt++;
	if(__ltdf2(-__inf(),__inf()) >= 0) errCnt++;
	if(__ltdf2(__inf(),3.14159) < 0) errCnt++;
	if(__ltdf2(3.14159,__inf()) >= 0) errCnt++;
	if(__ltdf2(-__inf(),3.14159) >= 0) errCnt++;
	if(__ltdf2(3.14159,-__inf()) < 0) errCnt++;
	// __ltsf2 returns a value less than zero if neither argument is NaN, and a is strictly less than b. 
	if(__ltsf2(2.71828f,3.14159f) >= 0) errCnt++;
	if(__ltsf2(3.14159f,3.14159f) < 0) errCnt++;
	if(__ltsf2(-2.71828f,-3.14159f) < 0) errCnt++;
	if(__ltsf2(nanf(NULL),nanf(NULL)) < 0) errCnt++;
	if(__ltsf2(nanf(NULL),3.14159f) < 0) errCnt++;
	if(__ltsf2(3.14159f,nanf(NULL)) < 0) errCnt++;
	if(__ltsf2(__inff(),__inff()) < 0) errCnt++;
	if(__ltsf2(-__inff(),-__inff()) < 0) errCnt++;
	if(__ltsf2(__inff(),-__inff()) < 0) errCnt++;
	if(__ltsf2(-__inff(),__inff()) >= 0) errCnt++;
	if(__ltsf2(__inff(),3.14159f) < 0) errCnt++;
	if(__ltsf2(3.14159f,__inff()) >= 0) errCnt++;
	if(__ltsf2(-__inff(),3.14159f) >= 0) errCnt++;
	if(__ltsf2(3.14159f,-__inff()) < 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE90 errCnt = %u\n",errCnt);
#endif

	// Test muldf3, mulsf3
	if(!fcntDoubleInRange(__muldf3(3.14159,2.71828),8.5397)) errCnt++;
	if(!isnan(__muldf3(nan(NULL),3.14159))) errCnt++;
	if(!isnan(__muldf3(3.14159,nan(NULL)))) errCnt++;
	vald = __muldf3(__inf(),__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __muldf3(__inf(),-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __muldf3(-__inf(),__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __muldf3(__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __muldf3(-__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __muldf3(3.14159,__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __muldf3(3.14159,-__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	if(!isnan(__muldf3(0.0,__inf()))) errCnt++;
	if(!isnan(__muldf3(__inf(),0.0))) errCnt++;
	if(!isnan(__muldf3(0.0,-__inf()))) errCnt++;
	if(!isnan(__muldf3(-__inf(),0.0))) errCnt++;
	if(!fcntFloatInRange(__mulsf3(3.14159f,2.71828f),8.5397f)) errCnt++;
	if(!isnan(__mulsf3(nanf(NULL),3.14159f))) errCnt++;
	if(!isnan(__mulsf3(3.14159f,nanf(NULL)))) errCnt++;
	valf = __mulsf3(__inff(),__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __mulsf3(-__inff(),-__inff()); 
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __mulsf3(__inff(),-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __mulsf3(-__inff(),__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __mulsf3(__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __mulsf3(-__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __mulsf3(3.14159f,__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __mulsf3(3.14159f,-__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	if(!isnan(__mulsf3(0.0f,__inff()))) errCnt++;
	if(!isnan(__mulsf3(__inff(),0.0f))) errCnt++;
	if(!isnan(__mulsf3(0.0f,-__inff()))) errCnt++;
	if(!isnan(__mulsf3(-__inf(),0.0f))) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE91 errCnt = %u\n",errCnt);
#endif

	// Test nedf2, nesf2
	// __nedf2 returns a nonzero value if either argument is NaN, or if a and b are unequal. 
	if(__nedf2(3.14159,2.71828) == 0) errCnt++;
	if(__nedf2(3.14159,3.14159) != 0) errCnt++;
	if(__nedf2(nan(NULL),nan(NULL)) == 0 ) errCnt++;
	if(__nedf2(nan(NULL),3.14159) == 0 ) errCnt++;
	if(__nedf2(3.14159,nan(NULL)) == 0 ) errCnt++;
	if(__nedf2(__inf(),__inf()) != 0 ) errCnt++;
	if(__nedf2(-__inf(),-__inf()) != 0 ) errCnt++;
	if(__nedf2(__inf(),-__inf()) == 0 ) errCnt++;
	if(__nedf2(-__inf(),__inf()) == 0 ) errCnt++;
	if(__nedf2(__inf(),3.14159) == 0 ) errCnt++;
	if(__nedf2(-__inf(),3.14159) == 0 ) errCnt++;
	if(__nedf2(3.14159,__inf()) == 0 ) errCnt++;
	if(__nedf2(3.14159,-__inf()) == 0 ) errCnt++;
	// __nesf2 returns a nonzero value if either argument is NaN, or if a and b are unequal. 
	if(__nesf2(3.14159f,2.71828f) == 0) errCnt++;
	if(__nesf2(3.14159f,3.14159f) != 0) errCnt++;
	if(__nesf2(nanf(NULL),nanf(NULL)) == 0 ) errCnt++;
	if(__nesf2(nanf(NULL),3.14159f) == 0 ) errCnt++;
	if(__nesf2(3.14159f,nanf(NULL)) == 0 ) errCnt++;
	if(__nesf2(__inff(),__inff()) != 0 ) errCnt++;
	if(__nesf2(-__inff(),-__inff()) != 0 ) errCnt++;
	if(__nesf2(__inff(),-__inff()) == 0 ) errCnt++;
	if(__nesf2(-__inff(),__inff()) == 0 ) errCnt++;
	if(__nesf2(__inff(),3.14159f) == 0 ) errCnt++;
	if(__nesf2(-__inff(),3.14159f) == 0 ) errCnt++;
	if(__nesf2(3.14159f,__inff()) == 0 ) errCnt++;
	if(__nesf2(3.14159f,-__inff()) == 0 ) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE92 errCnt = %u\n",errCnt);
#endif

	// Test negdf2, negsf2
	// __negdf2 returns the negation of a. It simply flips the sign bit, so it can produce negative zero and negative NaN. 
	if(!fcntDoubleInRange(__negdf2(3.14159),-3.14159)) errCnt++;
	if(signbit(__negdf2(nan(NULL))) == 0) errCnt++;
	if(signbit(__negdf2(__inf())) == 0) errCnt++;
	if(signbit(__negdf2(-__inf())) == 1) errCnt++;
	// __negsf2 returns the negation of a. It simply flips the sign bit, so it can produce negative zero and negative NaN. 
	if(!fcntFloatInRange(__negsf2(3.14159f),-3.14159f)) errCnt++;
	if(signbit(__negsf2(nanf(NULL))) == 0) errCnt++;
	if(signbit(__negsf2(__inff())) == 0) errCnt++;
	if(signbit(__negsf2(-__inff())) == 1) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE93 errCnt = %u\n",errCnt);
#endif

	// Test subdf3, subsf3
	if(!fcntDoubleInRange(__subdf3(3.14159,2.71828),0.42331)) errCnt++;
	if(!isnan(__subdf3(nan(NULL),3.14159))) errCnt++;
	if(!isnan(__subdf3(3.14159,nan(NULL)))) errCnt++;
	if(!isnan(__subdf3(__inf(),__inf()))) errCnt++;
	if(!isnan(__subdf3(-__inf(),-__inf()))) errCnt++;
	vald = __subdf3(__inf(),-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __subdf3(-__inf(),__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __subdf3(__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	vald = __subdf3(-__inf(),3.14159);
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __subdf3(3.14159,__inf());
	if(!isinf(vald) || signbit(vald) == 0) errCnt++;
	vald = __subdf3(3.14159,-__inf());
	if(!isinf(vald) || signbit(vald) != 0) errCnt++;
	if(!fcntFloatInRange(__subsf3(3.14159f,2.71828f),0.42331f)) errCnt++;
	if(!isnan(__subsf3(nan(NULL),3.14159f))) errCnt++;
	if(!isnan(__subsf3(3.14159f,nan(NULL)))) errCnt++;
	if(!isnan(__subsf3(__inff(),__inff()))) errCnt++;
	if(!isnan(__subsf3(-__inff(),-__inff()))) errCnt++;
	valf = __subsf3(__inff(),-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __subsf3(-__inff(),__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __subsf3(__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __subsf3(-__inff(),3.14159f);
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __subsf3(3.14159f,__inff());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;
	valf = __subsf3(3.14159f,-__inff());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE94 errCnt = %u\n",errCnt);
#endif

	// Test truncdfsf2
	if(!fcntFloatInRange(__truncdfsf2(3.14159),3.14159f)) errCnt++;
	if(!isnan(__truncdfsf2(nan(NULL)))) errCnt++;
	valf = __truncdfsf2(__inf());
	if(!isinf(valf) || signbit(valf) != 0) errCnt++;
	valf = __truncdfsf2(-__inf());
	if(!isinf(valf) || signbit(valf) == 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE95 errCnt = %u\n",errCnt);
#endif

	// Test ashldi3
	if(__ashldi3(0xA55A,8) != 0xA55A00) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE96 errCnt = %u\n",errCnt);
#endif

	// Test lshrdi3
	if(__lshrdi3(0xA55A,8) != 0xA5) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE97 errCnt = %u\n",errCnt);
#endif

	// Test divdi3
	if(__divdi3(64738L,8L) != 8092L) errCnt++;
	if(__divdi3(-64738L,8L) != -8092L) errCnt++;
	if(__divdi3(64738L,-8L) != -8092L) errCnt++;
	if(__divdi3(-64738L,-8L) != 8092L) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE98 errCnt = %u\n",errCnt);
#endif

	// Test moddi3
	if(__moddi3(64738L,8L) != 2L) errCnt++;
	if(__moddi3(-64738L,8L) != -2L) errCnt++;
	if(__moddi3(64738L,-8L) != 2L) errCnt++;
	if(__moddi3(-64738L,-8L) != -2L) errCnt++;

	// Test __muldi3
	if(__muldi3(64738L,8L) != 517904L) errCnt++;
	if(__muldi3(-64738L,8L) != -517904L) errCnt++;
	if(__muldi3(64738L,-8L) != -517904L) errCnt++;
	if(__muldi3(-64738L,-8L) != 517904L) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE99 errCnt = %u\n",errCnt);
#endif

	// Test __clzsi2
	if(__clzsi2(0x0F0F0F0F) != 4) errCnt++;
	if(__clzsi2(0xFF0F0F0F) != 0) errCnt++;
	if(__clzsi2(0x1F0F0F0F) != 3) errCnt++;
	if(__clzsi2(0x020F0F0F) != 6) errCnt++;

	// Test __ctzsi2 
	if(__ctzsi2(0x0F0F0F0F) != 0) errCnt++;
	if(__ctzsi2(0xFF0F0F00) != 8) errCnt++;
	if(__ctzsi2(0x1F0F0F02) != 1) errCnt++;
	if(__ctzsi2(0xA0000000) != 29) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE100 errCnt = %u\n",errCnt);
#endif

	return(errCnt);
}