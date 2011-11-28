#define DS1302_CE_PORT PORTA
#define DS1302_CE_PIN PINA
#define DS1302_CE_DDR DDRA
#define DS1302_CE_N 2
#define DS1302_IO_PORT PORTA
#define DS1302_IO_PIN PINA
#define DS1302_IO_DDR DDRA
#define DS1302_IO_N 1
#define DS1302_CLK_PORT PORTA
#define DS1302_CLK_PIN PINA
#define DS1302_CLK_DDR DDRA
#define DS1302_CLK_N 0

#define CS_PIN G
#define CS 3
#define DIN_PIN G
#define DIN 4
#define CLK_PIN G
#define CLK 2

#define CE_S SET(DS1302_CE_PORT,DS1302_CE_N);
#define CE_C CLR(DS1302_CE_PORT,DS1302_CE_N);
#define F_CPU 16000000UL  /* 16 MHz */
#include <util/delay.h>
#include "./max_7219/max_7219.h"

void _ds1302_w(uint16_t d)
{
	char i;
	d=d>>8|((d&0xff)<<8);
	SET(DS1302_IO_DDR,DS1302_IO_N);
	CLR(DS1302_CE_PORT,DS1302_CE_N);
	CLR(DS1302_CLK_PORT,DS1302_CLK_N);
	SET(DS1302_CE_PORT,DS1302_CE_N);
	for(i=0;i<16;i++){
		if(d&1){
			SET(DS1302_IO_PORT,DS1302_IO_N);
		} else {
			CLR(DS1302_IO_PORT,DS1302_IO_N);
		}
		/* _delay_us(250); */
		CLR(DS1302_CLK_PORT,DS1302_CLK_N);
		SET(DS1302_CLK_PORT,DS1302_CLK_N);
		/* _delay_us(250); */
		/* _delay_us(250); */
		d>>=1;
	}
	CLR(DS1302_CLK_PORT,DS1302_CLK_N);
	CLR(DS1302_CE_PORT,DS1302_CE_N);
}
uint8_t  _ds1302_r(uint8_t d)
{
	char i;
	uint8_t r=0;
	SET(DS1302_IO_DDR,DS1302_IO_N);
	CLR(DS1302_CE_PORT,DS1302_CE_N);
	CLR(DS1302_CLK_PORT,DS1302_CLK_N);
	SET(DS1302_CE_PORT,DS1302_CE_N);
	for(i=0;i<8;i++){
		if(d&1){
			SET(DS1302_IO_PORT,DS1302_IO_N);
		} else {
			CLR(DS1302_IO_PORT,DS1302_IO_N);
		}
		/* _delay_us(250); */
		CLR(DS1302_CLK_PORT,DS1302_CLK_N);
		SET(DS1302_CLK_PORT,DS1302_CLK_N);
		/* _delay_us(250); */
		/* _delay_us(250); */
		d>>=1;
	}
	CLR(DS1302_IO_DDR,DS1302_IO_N);
	SET(DS1302_IO_PORT,DS1302_IO_N);
	CLR(DS1302_CLK_PORT,DS1302_CLK_N);
	for(i=0;i<8;i++){
		SET(DS1302_CLK_PORT,DS1302_CLK_N);
		CLR(DS1302_CLK_PORT,DS1302_CLK_N);
		if((DS1302_IO_PIN&(1<<DS1302_IO_N))){
			r|=1<<i;
		}
		
		/* _delay_us(100); */
		/* _delay_us(150); */

	}
	CLR(DS1302_CE_PORT,DS1302_CE_N);
	CLR(DS1302_CLK_PORT,DS1302_CLK_N);
	return r;
}


int main(void)
{
	unsigned char t,t_;
	long long i=0;
	max_7219_init();
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_on();
	_max_7219_scan_limit(1);
	_max_7219_intensity(5);
	SET(DS1302_CLK_DDR,DS1302_CLK_N);
	SET(DS1302_CE_DDR,DS1302_CE_N);
	SET(DS1302_IO_DDR,DS1302_IO_N);
	/* SET(DS1302_CE_PORT,DS1302_CE_N); */
	/* SET(DS1302_IO_PORT,DS1302_IO_N); */
	/* SET(DS1302_CLK_PORT,DS1302_CLK_N); */
	_ds1302_w((0x8e<<8)|0x00);
	_ds1302_w((0x80<<8)|0x80);
	_ds1302_w((0x80<<8)|0x00);
	t_=0x60;
	while(1){
		t=_ds1302_r(0x81);
		if(t!=0xff){
			if(t!=t_){
				max_7219_number(i++);
				t_=t;
			}
		}
		_delay_ms(490);
		/* _ds1302_w((0x8e<<8)|0x80); */
	}
	
	return 0;
}
