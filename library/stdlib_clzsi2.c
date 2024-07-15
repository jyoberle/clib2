#ifndef _STDLIB_HEADERS_H
#include "stdlib_headers.h"
#endif /* _STDLIB_HEADERS_H */

#if defined(__GNUC__)

/****************************************************************************/

// implemented based on https://blog.stephencleary.com/2010/10/implementing-gccs-builtin-functions.html
__attribute__((externally_visible)) LONG __clzsi2(ULONG x)
{
  // This uses a binary search (counting down) algorithm from Hacker's Delight.
   ULONG y;
   LONG n = 32;
   y = x >>16;  if (y != 0) {n = n -16;  x = y;}
   y = x >> 8;  if (y != 0) {n = n - 8;  x = y;}
   y = x >> 4;  if (y != 0) {n = n - 4;  x = y;}
   y = x >> 2;  if (y != 0) {n = n - 2;  x = y;}
   y = x >> 1;  if (y != 0) return n - 2;
   return n - x;
}

/****************************************************************************/

#endif /* __GNUC__ */