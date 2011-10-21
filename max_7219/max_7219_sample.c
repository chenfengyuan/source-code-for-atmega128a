/* Copyright (C) 2011 by ChenFengyuan */

/* Permission is hereby granted, free of charge, to any person obtaining a copy */
/* of this software and associated documentation files (the "Software"), to deal */
/* in the Software without restriction, including without limitation the rights */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell */
/* copies of the Software, and to permit persons to whom the Software is */
/* furnished to do so, subject to the following conditions: */

/* The above copyright notice and this permission notice shall be included in */
/* all copies or substantial portions of the Software. */

/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN */
/* THE SOFTWARE. */

#define F_CPU 16000000UL  /* 16 MHz */
#include <util/delay.h>
#include <avr/io.h>

#define CS_PIN G
#define CS 3
#define DIN_PIN B
#define DIN 4
#define CLK_PIN C
#define CLK 2


#include "dis.h"

int main(void)
{
	char i;
	/* PIN initial */
	max_7219_init();
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_on();
	_max_7219_intensity(5);
	
	while(1){
		max_7219_number(42);
		_delay_ms(1000);
		max_7219_clear();
		max_7219_decode_all_on();
		_max_7219_scan_limit(4);
		_max_7219_dn(4,4,0);
		_max_7219_dn(0xf,3,0);
		_max_7219_dn(0xf,2,1);
		_max_7219_dn(0xf,1,0);
		_max_7219_dn(2,0,1);
		_delay_ms(1000);

		max_7219_clear();
		max_7219_decode_all_off();
		max_7219_hex_number(0x12345680);
		_delay_ms(1000);
		max_7219_hex_number(0x79abcdef);
		_delay_ms(5000);
		max_7219_hex_number(-0x1234567);
		_delay_ms(1000);
		for(i=0;i<8;++i){
			max_7219_hex_and_shift_left(i);
			_delay_ms(1000);
		}
		max_7219_clear_shift_count();
		for(i=8;i<16;++i){
			max_7219_hex_and_shift_left(i);
			_delay_ms(1000);
		}
	}
	return 0;
}

