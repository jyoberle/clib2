// Checks the mathlib support functions
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
	if(!fcntDoubleInRange(acos(0.5),1.04719)) errCnt++;
	if(!fcntFloatInRange(acos(0.5f),1.04719f)) errCnt++;
	if(!fcntDoubleInRange(acosh(1.256),0.70109)) errCnt++;
	if(!fcntFloatInRange(acoshf(1.256f),0.70109f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE14 errCnt = %u\n",errCnt);
#endif

	// Test asin, asin, asinh, asinhf
	if(!fcntDoubleInRange(asin(0.5),0.5235)) errCnt++;
	if(!fcntFloatInRange(asin(0.5f),0.5235f)) errCnt++;
	if(!fcntDoubleInRange(asinh(1.256),1.0513)) errCnt++;
	if(!fcntFloatInRange(asinhf(1.256f),1.0513f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE15 errCnt = %u\n",errCnt);
#endif

	// Test atan, atan, atanh, atanhf
	if(!fcntDoubleInRange(atan(0.5),0.4636)) errCnt++;
	if(!fcntFloatInRange(atan(0.5f),0.4636f)) errCnt++;
	if(!fcntDoubleInRange(atanh(0.256),0.2618)) errCnt++;
	if(!fcntFloatInRange(atanhf(0.256f),0.2618f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE16 errCnt = %u\n",errCnt);
#endif

	// Test atan2, atan2f
	if(!fcntDoubleInRange(atan2(1.0,1.0),0.7853)) errCnt++;
	if(!fcntDoubleInRange(atan2(-1.0,-1.0),-2.3561)) errCnt++;
	if(!fcntFloatInRange(atan2f(1.0f,1.0f),0.7853f)) errCnt++;
	if(!fcntFloatInRange(atan2f(-1.0f,-1.0f),-2.3561f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE17 errCnt = %u\n",errCnt);
#endif

	// Test cbrt, cbrtf
	if(!fcntDoubleInRange(cbrt(1969.1969),12.5341)) errCnt++;
	if(!fcntFloatInRange(cbrtf(1969.1969f),12.5341f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE18 errCnt = %u\n",errCnt);
#endif

	// Test ceil, ceilf
	if(!fcntDoubleInRange(ceil(1971.1971),1972.0)) errCnt++;
	if(!fcntFloatInRange(ceilf(1971.1971f),1972.0f)) errCnt++;

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
	if(!fcntDoubleInRange(cos(0.5),0.8775)) errCnt++;
	if(!fcntFloatInRange(cosf(0.5f),0.8775f)) errCnt++;
	if(!fcntDoubleInRange(cosh(1.256),1.8980)) errCnt++;
	if(!fcntFloatInRange(coshf(1.256f),1.8980f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE21 errCnt = %u\n",errCnt);
#endif

	// Test erf, erff, erf, erfcf
	if(!fcntDoubleInRange(erf(0.5),0.5205)) errCnt++;
	if(!fcntFloatInRange(erff(0.5f),0.5205f)) errCnt++;
	if(!fcntDoubleInRange(erfc(0.5),0.4795)) errCnt++;
	if(!fcntFloatInRange(erfcf(0.5f),0.4795f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE22 errCnt = %u\n",errCnt);
#endif

	// Test exp, expf, exp2, exp2f
	if(!fcntDoubleInRange(exp(3.14159),23.1406)) errCnt++;
	if(!fcntFloatInRange(expf(3.14159f),23.1406f)) errCnt++;
	if(!fcntDoubleInRange(exp2(3.14159),8.8249)) errCnt++;
	if(!fcntFloatInRange(exp2f(3.14159f),8.8249f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE23 errCnt = %u\n",errCnt);
#endif

	// Test expm1, expm1f
	if(!fcntDoubleInRange(expm1(3.14159),22.1406)) errCnt++;
	if(!fcntFloatInRange(expm1f(3.14159f),22.1406f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE24 errCnt = %u\n",errCnt);
#endif

	// Test fabs, fabsf
	if(!fcntDoubleInRange(fabs(-3.14159),3.14159)) errCnt++;
	if(!fcntDoubleInRange(fabs(3.14159),3.14159)) errCnt++;
	if(!fcntFloatInRange(fabsf(-3.14159f),3.14159f)) errCnt++;
	if(!fcntFloatInRange(fabsf(3.14159f),3.14159f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE25 errCnt = %u\n",errCnt);
#endif

	// Test fdim, fdimf
	if(!fcntDoubleInRange(fdim(3.6,3.5),0.1)) errCnt++;
	if(!fcntDoubleInRange(fdim(3.5,3.6),0.0)) errCnt++;
	if(!fcntFloatInRange(fdimf(3.6f,3.5f),0.1f)) errCnt++;
	if(!fcntFloatInRange(fdimf(3.5f,3.6f),0.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE26 errCnt = %u\n",errCnt);
#endif

	// Test floor, floorf
	if(!fcntDoubleInRange(floor(0.5),0.0)) errCnt++;
	if(!fcntDoubleInRange(floor(-0.5),-1.0)) errCnt++;
	if(!fcntFloatInRange(floorf(0.5f),0.0f)) errCnt++;
	if(!fcntFloatInRange(floorf(-0.5f),-1.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE27 errCnt = %u\n",errCnt);
#endif

	// Test fma, fmaf
	if(!fcntDoubleInRange(fma(-1.1,-2.2,3.3),5.72)) errCnt++;
	if(!fcntFloatInRange(fmaf(-1.1f,-2.2f,3.3f),5.72f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE28 errCnt = %u\n",errCnt);
#endif

	// Test fmax, fmaxf
	if(!fcntDoubleInRange(fmax(-1.1,-2.2),-1.1)) errCnt++;
	if(!fcntFloatInRange(fmaxf(-1.1f,-2.2f),-1.1f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE29 errCnt = %u\n",errCnt);
#endif

	// Test fmin, fminf
	if(!fcntDoubleInRange(fmin(-1.1,-2.2),-2.2)) errCnt++;
	if(!fcntFloatInRange(fminf(-1.1f,-2.2f),-2.2f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE30 errCnt = %u\n",errCnt);
#endif

	// Test fmod, fmodf
	if(!fcntDoubleInRange(fmod(1677216.25,8192.25),5997.25)) errCnt++;
	if(!fcntFloatInRange(fmodf(1677216.25f,8192.25f),5997.25f)) errCnt++;

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
	double fraction = frexp(2029.0,&exp);
	if(!fcntDoubleInRange(fraction,0.990723) || exp != 11) errCnt++;
	double fractionf = frexpf(2029.0f,&exp);
	if(!fcntFloatInRange(fractionf,0.990723f) || exp != 11) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE33 errCnt = %u\n",errCnt);
#endif

	// Test hypot, hypotf
	if(!fcntDoubleInRange(hypot(11.25,9.33),14.6155)) errCnt++;
	if(!fcntFloatInRange(hypotf(11.25f,9.33f),14.6155f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE34 errCnt = %u\n",errCnt);
#endif

	// Test ilogb, ilogbf
	if(ilogb(123.45) != 6) errCnt++;
	if(ilogbf(123.45f) != 6) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE35 errCnt = %u\n",errCnt);
#endif

	// Test ldexp, ldexpf
	if(!fcntDoubleInRange(ldexp(1971.04,4),31536.64)) errCnt++;
	if(!fcntFloatInRange(ldexpf(1971.04f,4),31536.64f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE36 errCnt = %u\n",errCnt);
#endif

	// Test lgamma, lgammaf
	if(!fcntDoubleInRange(lgamma(10.0),12.801827)) errCnt++;
	if(!fcntFloatInRange(lgammaf(10.0f),12.801827f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE37 errCnt = %u\n",errCnt);
#endif

	// Test lrint, lrintf, llrint
	if(lrint(1969.1976) != 1969UL) errCnt++;
	if(lrint(1969.9999) != 1970UL) errCnt++;
	if(lrintf(1969.1976f) != 1969UL) errCnt++;
	if(lrintf(1969.9999f) != 1970UL) errCnt++;
#if defined(USE_64_BIT_INTS)
	if(llrint(1969.1976) != 1969ULL) errCnt++;
	if(llrint(1969.9999) != 1970ULL) errCnt++;
#endif

#if OPTION_TRACE_MATH
	printf("TRACE38 errCnt = %u\n",errCnt);
#endif

	// Test log, log10, log10f, log1p, log1pf, log2, log2f, logb, logbf, logf
	if(!fcntDoubleInRange(log(4.0),1.3862)) errCnt++;
	if(!fcntFloatInRange(logf(4.0f),1.3862f)) errCnt++;
	if(!fcntDoubleInRange(log10(4.0),0.6020)) errCnt++;
	if(!fcntFloatInRange(log10(4.0f),0.6020f)) errCnt++;
	if(!fcntDoubleInRange(log1p(4.0),1.6094)) errCnt++;
	if(!fcntFloatInRange(log1pf(4.0f),1.6094f)) errCnt++;
	if(!fcntDoubleInRange(log2(6.0),2.58496)) errCnt++;
	if(!fcntFloatInRange(log2f(6.0f),2.58496f)) errCnt++;
	if(!fcntDoubleInRange(logb(8.0),3.0)) errCnt++;
	if(!fcntFloatInRange(logbf(8.0f),3.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE39 errCnt = %u\n",errCnt);
#endif

	// Test lround, lroundf
	if(lround(4.9) != 5) errCnt++;
	if(lround(4.4) != 4) errCnt++;
	if(lroundf(4.9f) != 5) errCnt++;
	if(lroundf(4.4f) != 4) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE40 errCnt = %u\n",errCnt);
#endif

	// Test modf, modff
	double modfi;
	float modffi;
	if(!fcntDoubleInRange(modf(4.159,&modfi),0.159)) errCnt++;
	if(!fcntDoubleInRange(modfi,4.0)) errCnt++;
	if(!fcntFloatInRange(modff(4.159f,&modffi),0.159)) errCnt++;
	if(!fcntFloatInRange(modffi,4.0f)) errCnt++;

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
	if(!fcntDoubleInRange(nearbyint(4.159),4.0)) errCnt++;
	if(!fcntDoubleInRange(nearbyint(4.95),5.0)) errCnt++;
	if(!fcntFloatInRange(nearbyintf(4.159f),4.0f)) errCnt++;
	if(!fcntFloatInRange(nearbyintf(4.95f),5.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE43 errCnt = %u\n",errCnt);
#endif

	// Test nextafter, nextafterf
	if(!fcntDoubleInRange(nextafter(1.0,1.1),1.000000001)) errCnt++; // fcntDoubleInRange does not check delta below 0.001
	if(!fcntFloatInRange(nextafterf(1.0f,1.1f),1.000000001f)) errCnt++; // fcntFloatInRange does not check delta below 0.001f

#if OPTION_TRACE_MATH
	printf("TRACE44 errCnt = %u\n",errCnt);
#endif

	// Test pow, powf
	if(!fcntDoubleInRange(pow(3.14159,3.14159),36.461952)) errCnt++;
	if(!fcntFloatInRange(powf(3.14159f,3.14159f),36.461952f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE45 errCnt = %u\n",errCnt);
#endif

	// Test remainder, remainderf
	if(!fcntDoubleInRange(remainder(5.1,3.0),-0.9)) errCnt++;
	if(!fcntFloatInRange(remainderf(5.1f,3.0f),-0.9f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE46 errCnt = %u\n",errCnt);
#endif

	// Test remquo, remquof
	int quotient;
	double remainder;
	float remainderf;

	remainder = remquo(12.5,2.2,&quotient);
	if(!fcntDoubleInRange(remainder,-0.7) || quotient != 6) errCnt++;
	remainderf = remquof(12.5f,2.2f,&quotient);
	if(!fcntFloatInRange(remainderf,-0.7f) || quotient != 6) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE47 errCnt = %u\n",errCnt);
#endif

	// Test rint, rintf
	if(!fcntDoubleInRange(rint(4.159),4.0)) errCnt++;
	if(!fcntDoubleInRange(rint(4.95),5.0)) errCnt++;
	if(!fcntFloatInRange(rintf(4.159f),4.0f)) errCnt++;
	if(!fcntFloatInRange(rintf(4.95f),5.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE48 errCnt = %u\n",errCnt);
#endif

	// Test round, roundf
	if(!fcntDoubleInRange(round(5.1),5.0)) errCnt++;
	if(!fcntDoubleInRange(round(5.6),6.0)) errCnt++;
	if(!fcntDoubleInRange(round(-5.6),-6.0)) errCnt++;
	if(!fcntFloatInRange(roundf(5.1f),5.0f)) errCnt++;
	if(!fcntFloatInRange(roundf(5.6f),6.0f)) errCnt++;
	if(!fcntFloatInRange(roundf(-5.6f),-6.0f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE49 errCnt = %u\n",errCnt);
#endif

	// Test scalbn, scalbnf
	if(!fcntDoubleInRange(scalbn(1971.1969,5L),63078.3008)) errCnt++;
	if(!fcntFloatInRange(scalbn(1971.1969f,5L),63078.3008f)) errCnt++;

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
	if(!fcntDoubleInRange(sin(0.5),0.4794)) errCnt++;
	if(!fcntFloatInRange(sinf(0.5f),0.4794f)) errCnt++;
	if(!fcntDoubleInRange(sinh(1.256),1.6132)) errCnt++;
	if(!fcntFloatInRange(sinhf(1.256f),1.6132f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE52 errCnt = %u\n",errCnt);
#endif

	// Test sqrt, sqrtf
	if(!fcntDoubleInRange(sqrt(0.5),0.7071)) errCnt++;
	if(!fcntFloatInRange(sqrtf(0.5f),0.7071f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE53 errCnt = %u\n",errCnt);
#endif

	// Test tan, tanf, tanh, tanhf
	if(!fcntDoubleInRange(tan(0.5),0.5463)) errCnt++;
	if(!fcntFloatInRange(tanf(0.5f),0.5463f)) errCnt++;
	if(!fcntDoubleInRange(tanh(1.256),0.8499)) errCnt++;
	if(!fcntFloatInRange(tanhf(1.256f),0.8499f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE54 errCnt = %u\n",errCnt);
#endif

	// Test tgamma, tgammaf
	if(!fcntDoubleInRange(tgamma(0.5),1.772454)) errCnt++;
	if(!fcntFloatInRange(tgammaf(0.5f),1.772454f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE55 errCnt = %u\n",errCnt);
#endif

	// Test trunc, trunf
	if(!fcntDoubleInRange(trunc(1971.1969),1971.0)) errCnt++;
	if(!fcntFloatInRange(truncf(1971.1969f),1971.0f)) errCnt++;

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
	printf("TRAC66 errCnt = %u\n",errCnt);
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
	if(!fcntFloatInRange(__addsf3(3.14159f,2.71828f),5.85987f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE80 errCnt = %u\n",errCnt);
#endif

	// Test divdf3, divsf3
	if(!fcntDoubleInRange(__divdf3(3.14159,2.71828),1.15572)) errCnt++;
	if(!fcntFloatInRange(__divsf3(3.14159f,2.71828f),1.15572f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE81 errCnt = %u\n",errCnt);
#endif

	// Test eqdf2, eqsf2
	if(__eqdf2(3.14159,2.71828) == 0) errCnt++;
	if(__eqdf2(2.71828,3.14159) == 0) errCnt++;
	if(__eqdf2(3.14159,3.14159) != 0) errCnt++;
	if(__eqsf2(3.14159f,2.71828f) == 0) errCnt++;
	if(__eqsf2(2.71828f,3.14159f) == 0) errCnt++;
	if(__eqsf2(3.14159f,3.14159f) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE82 errCnt = %u\n",errCnt);
#endif

	// Test extendsfdf2
	if(!fcntDoubleInRange(__extendsfdf2(2.71828f),2.71828)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE83 errCnt = %u\n",errCnt);
#endif

	// Test fixdfsi, fixsfsi
	if(__fixdfsi(-3.14159) != -3L) errCnt++;
	if(__fixsfsi(-3.14159f) != -3L) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE84 errCnt = %u\n",errCnt);
#endif

	// Test fixunsdfsi, fixunssfsi
	if(__fixunsdfsi(-3.14159) != 0) errCnt++;
	if(__fixunsdfsi(3.14159) != 3) errCnt++;
	if(__fixunssfsi(-3.14159f) != 0) errCnt++;
	if(__fixunssfsi(3.14159f) != 3) errCnt++;

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
	if(__gedf2(3.14159,2.71828) < 0) errCnt++;
	if(__gedf2(3.14159,3.14159) < 0) errCnt++;
	if(__gedf2(-3.14159,-2.71828) >= 0) errCnt++;
	if(__gesf2(3.14159f,2.71828f) < 0) errCnt++;
	if(__gesf2(3.14159f,3.14159f) < 0) errCnt++;
	if(__gesf2(-3.14159f,-2.71828f) >= 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE87 errCnt = %u\n",errCnt);
#endif

	// Test gtdf2, gtsf2
	if(__gtdf2(3.14159,2.71828) <= 0) errCnt++;
	if(__gtdf2(3.14159,3.14159) > 0) errCnt++;
	if(__gtdf2(-3.14159,-2.71828) > 0) errCnt++;
	if(__gtsf2(3.14159f,2.71828f) <= 0) errCnt++;
	if(__gtsf2(3.14159f,3.14159f) > 0) errCnt++;
	if(__gtsf2(-3.14159f,-2.71828f) > 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE88 errCnt = %u\n",errCnt);
#endif

	// Test ledf2, lesf2
	if(__ledf2(2.71828,3.14159) > 0) errCnt++;
	if(__ledf2(3.14159,3.14159) > 0) errCnt++;
	if(__ledf2(-2.71828,-3.14159) <= 0) errCnt++;
	if(__lesf2(2.71828f,3.14159f) > 0) errCnt++;
	if(__lesf2(3.14159f,3.14159f) > 0) errCnt++;
	if(__lesf2(-2.71828f,-3.14159f) <= 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE89 errCnt = %u\n",errCnt);
#endif

	// Test ltdf2, ltsf2
	if(__ltdf2(2.71828,3.14159) >= 0) errCnt++;
	if(__ltdf2(3.14159,3.14159) < 0) errCnt++;
	if(__ltdf2(-2.71828,-3.14159) < 0) errCnt++;
	if(__ltsf2(2.71828f,3.14159f) >= 0) errCnt++;
	if(__ltsf2(3.14159f,3.14159f) < 0) errCnt++;
	if(__ltsf2(-2.71828f,-3.14159f) < 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE90 errCnt = %u\n",errCnt);
#endif

	// Test muldf3, mulsf3
	if(!fcntDoubleInRange(__muldf3(3.14159,2.71828),8.5397)) errCnt++;
	if(!fcntFloatInRange(__mulsf3(3.14159f,2.71828f),8.5397f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE91 errCnt = %u\n",errCnt);
#endif

	// Test nedf2, nesf2
	if(__nedf2(3.14159,2.71828) == 0) errCnt++;
	if(__nedf2(3.14159,3.14159) != 0) errCnt++;
	if(__nesf2(3.14159f,2.71828f) == 0) errCnt++;
	if(__nesf2(3.14159f,3.14159f) != 0) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE92 errCnt = %u\n",errCnt);
#endif

	// Test negdf2, negsf2
	if(!fcntDoubleInRange(__negdf2(3.14159),-3.14159)) errCnt++;
	if(!fcntFloatInRange(__negsf2(3.14159f),-3.14159f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE93 errCnt = %u\n",errCnt);
#endif

	// Test subdf3, subsf3
	if(!fcntDoubleInRange(__subdf3(3.14159,2.71828),0.42331)) errCnt++;
	if(!fcntFloatInRange(__subsf3(3.14159f,2.71828f),0.42331f)) errCnt++;

#if OPTION_TRACE_MATH
	printf("TRACE94 errCnt = %u\n",errCnt);
#endif

	// Test truncdfsf2
	if(!fcntFloatInRange(__truncdfsf2(3.14159),3.14159f)) errCnt++;	

#if OPTION_TRACE_MATH
	printf("TRACE95 errCnt = %u\n",errCnt);
#endif

	return(errCnt);
}