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

#ifndef _MAX_7219_H
#define _MAX_7219_H
#include <avr/io.h>


#define CLR(BYTE,BIT) ((BYTE)&=~(1<<(BIT)))
#define SET(BYTE,BIT) ((BYTE)|=(1<<(BIT)))

#define _PORT(x) PORT ## x
#define _DDR(x) DDR ## x
#define PORT(x) _PORT(x)
#define DDR(x) _DDR(x)
#define CS_PORT PORT(CS_PIN)
#define DIN_PORT PORT(DIN_PIN)
#define CLK_PORT PORT(CLK_PIN)
#define CS_DDR DDR(CS_PIN)
#define DIN_DDR DDR(DIN_PIN)
#define CLK_DDR DDR(CLK_PIN)


#define SCS SET(CS_PORT,CS)
#define SCLK SET(CLK_PORT,CLK)
#define SDIN SET(DIN_PORT,DIN)
#define CCS CLR(CS_PORT,CS)
#define CCLK CLR(CLK_PORT,CLK)
#define CDIN CLR(DIN_PORT,DIN)

/* basis */

uint8_t _max_7219_decode=0;

void _max_7219_si(uint16_t d)
{
	char i;
	CCLK;
	CCS;
	for(i=15;i>=0;--i){
		if(d&(1<<i)){
			SDIN;
		} else {
			CDIN;
		}
		
		SCLK;
		CCLK;
	}
	SCS;
}

	
void  _max_7219_noop(void)
{
	_max_7219_si(0x0);
}

void _max_7219_dn(char d,char n,char dot)
{
	_max_7219_si((n+1)<<8|dot<<7|d);
}
void _max_7219_decode_mode(uint8_t d)
{
	_max_7219_si(0x9<<8|d);
}
void _max_7219_intensity(uint8_t d)
{
	_max_7219_si(0xa<<8|d);
}
void _max_7219_scan_limit(uint8_t d)
{
	_max_7219_si(0xb<<8|d);
}
void _max_7219_shutdown(uint8_t d)
{
	_max_7219_si(0xc<<8|d);
}
void _max_7219_test(uint8_t d)
{
	_max_7219_si(0xf<<8|d);
}

/* extended */

void max_7219_test_on(void)
{
	_max_7219_test(1);
}

void max_7219_test_off(void)
{
	_max_7219_test(0);
}

void max_7219_decode_all_on(void)
{
	_max_7219_decode_mode((1<<8)-1);
}

void max_7219_decode_all_off(void)
{
	_max_7219_decode_mode(0);
}

void max_7219_decode_on(uint8_t n)
{
	SET(_max_7219_decode,n);
	_max_7219_decode_mode(_max_7219_decode);
}

void max_7219_decode_off(uint8_t n)
{
	CLR(_max_7219_decode,n);
	_max_7219_decode_mode(_max_7219_decode);
}

void max_7219_on(void)
{
	_max_7219_shutdown(1);
}
void max_7219_off(void)
{
	_max_7219_shutdown(0);
}
void max_7219_clear(void)
{
	int i;
	_max_7219_decode_mode(0);
	for(i=0;i<8;++i){
		_max_7219_dn(0,i,0);
	}
}

int max_7219_number(int32_t d)
{
	char a[8]={0};
	int i;
	char flag;
	char n=0;
	if(d<0){
		flag=1;
		d=-d;
	} else {
		flag=0;
	}
	for(i=0;i<8 && d ;++i){
		a[i]=(d%10);
		d/=10;
		++n;
	}
	n=n+flag;
	if(n>8)
		return 1;
	if(flag)
		a[n-1]=0xa;
	_max_7219_scan_limit(n-1);
	/* _max_7219_decode_mode((1<<n)-1); */
	for(i=0;i<n;++i){
		_max_7219_dn(a[i],i,0);
	}
	return 0;
}

int max_7219_hex(uint8_t d,uint8_t n)
{
	char c;
	switch(d){
	case 0:c=0x7e;
		break;
	case 1:c=0x30;
		break;
	case 2:c=0x6d;
		break;
	case 3:c=0x79;
		break;
	case 4:c=0x33;
		break;
	case 5:c=0x5b;
		break;
	case 6:c=0x5f;
		break;
	case 7:c=0x70;
		break;
	case 8:c=0x7f;
		break;
	case 9:c=0x7b;
		break;
	case 10:c=0x77;
		break;
	case 11:c=0x1f;
		break;
	case 12:c=0x4e;
		break;
	case 13:c=0x3d;
		break;
	case 14:c=0x4f;
		break;
	case 15:c=0x47;
		break;
	default:return 1;
		break;
	}
	_max_7219_dn(c,n,0);
	return 0;
}


int max_7219_hex_number(int32_t d)
{
	char a[8]={0};
	int i;
	char flag;
	char n;
	if(d<0){
		n=flag=1;
		d=-d;
	} else {
		flag=n=0;
	}
	for(i=0;i<8 && d ;++i){
		a[i]=(d%16);
		d/=16;
		++n;
	}
	if(n>8)
		return 1;
	_max_7219_scan_limit(n-1);
	if(flag){
		_max_7219_dn(1,n-1,0);
		for(i=0;i<n-1;++i){
			max_7219_hex(a[i],i);
		}
	} else {
		for(i=0;i<n;++i){
			max_7219_hex(a[i],i);
		}
	}
	return 0;
}

char _max_7219_shift_count=0;

void max_7219_clear_shift_count(void)
{
	_max_7219_shift_count=0;
}

void max_7219_hex_and_shift_left(uint8_t d)
{
	static uint32_t num=0x0;
	uint32_t temp;
	char i;
	temp=num=(num<<4)|d;
	
	if(_max_7219_shift_count<8)
		++_max_7219_shift_count;
	_max_7219_scan_limit(_max_7219_shift_count-1);
	for(i=0;i<_max_7219_shift_count;++i){
		max_7219_hex(temp&0xf,i);
		temp>>=4;
	}
}

void max_7219_init(void)
{
	SET(CS_DDR,CS);
	SET(CLK_DDR,CLK);
	SET(DIN_DDR,DIN);
	SET(CS_PORT,CS);
	SET(CLK_PORT,CLK);
	SET(DIN_PORT,DIN);
}

#endif
