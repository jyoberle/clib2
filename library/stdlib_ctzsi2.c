#ifndef _STDLIB_HEADERS_H
#include "stdlib_headers.h"
#endif /* _STDLIB_HEADERS_H */

#if defined(__GNUC__)

/****************************************************************************/

// implemented based on https://blog.stephencleary.com/2010/10/implementing-gccs-builtin-functions.html
__attribute__((externally_visible)) LONG __ctzsi2(ULONG x)
{
  // This uses a binary search algorithm from Hacker's Delight.
  LONG n = 1;
  if ((x & 0x0000FFFF) == 0) {n = n +16; x = x >>16;}
  if ((x & 0x000000FF) == 0) {n = n + 8; x = x >> 8;}
  if ((x & 0x0000000F) == 0) {n = n + 4; x = x >> 4;}
  if ((x & 0x00000003) == 0) {n = n + 2; x = x >> 2;}
  return n - (x & 1);
}

/****************************************************************************/

#endif /* __GNUC__ */