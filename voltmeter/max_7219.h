#ifndef _MAX_7219_H
#define _MAX_7219_H 1
#include <avr/io.h>
#include "util.h"

extern void (*trans)(uint16_t data);
void max_7219_scan_limit(uint8_t d);
void max_7219_intensity(uint8_t d);
void max_7219_dn(char d,char n,char dot);
void max_7219_test_on(void);
void max_7219_test_off(void);
void max_7219_decode_all_on(void);
void max_7219_decode_all_off(void);
void max_7219_decode_on(uint8_t n);
void max_7219_decode_off(uint8_t n);
void max_7219_on(void);
void max_7219_off(void);
void max_7219_clear(void);
int max_7219_number(int32_t d);
int max_7219_hex(uint8_t d,uint8_t n);
int max_7219_hex_number(int32_t d);
void max_7219_clear_shift_count(void);
void max_7219_hex_and_shift_left(uint8_t d);
#endif
