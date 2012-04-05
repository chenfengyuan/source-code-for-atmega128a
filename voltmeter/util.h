#ifndef _UTIL_H
#define _UTIL_H 1
#define CLR(BYTE,BIT) ((BYTE)&=~(1<<(BIT)))
#define SET(BYTE,BIT) ((BYTE)|=(1<<(BIT)))
#define _PORT(x) PORT ## x
#define _DDR(x) DDR ## x
#define PORT(x) _PORT(x)
#define DDR(x) _DDR(x)
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif
