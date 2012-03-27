#ifndef _UTIL_H
#define _UTIL_H 1
#define CLR(BYTE,BIT) ((BYTE)&=~(1<<(BIT)))
#define SET(BYTE,BIT) ((BYTE)|=(1<<(BIT)))
#endif
