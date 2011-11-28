#ifndef _DS_1302_H
#define _DS_1302_H

#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL  // 16 MHz
#endif
#include <util/delay.h>

#ifndef CLR
#define CLR(BYTE,BIT) ((BYTE)&=~(1<<(BIT)))
#endif

#ifndef SET
#define SET(BYTE,BIT) ((BYTE)|=(1<<(BIT)))
#endif

#ifndef _PORT
#define _PORT(x) PORT ## x
#endif
#ifndef _DDR
#define _DDR(x) DDR ## x
#endif
#ifndef _PIN
#define _PIN(x) PIN ## x
#endif

#ifndef PORT
#define PORT(x) _PORT(x)
#endif
#ifndef DDR
#define DDR(x) _DDR(x)
#endif
#ifndef PIN
#define PIN(x) _PIN(x)
#endif

#define DS_CE_PORT PORT(DS_CE_PIN)
#define DS_CLK_PORT PORT(DS_CLK_PIN)
#define DS_IO_PORT PORT(DS_IO_PIN)
#define DS_CE_DDR DDR(DS_CE_PIN)
#define DS_CLK_DDR DDR(DS_CLK_PIN)
#define DS_IO_DDR DDR(DS_IO_PIN)

#define DS_CE_IN PIN(DS_CE_PIN)
#define DS_CLK_IN PIN(DS_CLK_PIN)
#define DS_IO_IN PIN(DS_IO_PIN)


#define SDS_CE SET(DS_CE_PORT,DS_CE)
#define SDS_CLK SET(DS_CLK_PORT,DS_CLK)
#define SDS_IO SET(DS_IO_PORT,DS_IO)
#define CDS_CE CLR(DS_CE_PORT,DS_CE)
#define CDS_CLK CLR(DS_CLK_PORT,DS_CLK)
#define CDS_IO CLR(DS_IO_PORT,DS_IO)

/* base */

void _ds1302_w(uint16_t d)
{
	char i;
	SET(DS_CE_DDR,DS_CE);
	SET(DS_CLK_DDR,DS_CLK);
	SET(DS_IO_DDR,DS_IO);
	CDS_CE;
	CDS_CLK;
	SDS_CE;
	for(i=16;i<=0;i--){
		if(d&(1<<i)){
			SDS_IO;
		} else {
			CDS_IO;
		}
		SDS_CLK;
		_delay_us(250);
		CDS_CLK;
		_delay_us(250);
	}
	CDS_CE;
}

uint8_t  _ds1302_r(uint8_t d)
{
	char i;
	uint8_t r=0;
	SET(DS_CE_DDR,DS_CE);
	SET(DS_CLK_DDR,DS_CLK);
	SET(DS_IO_DDR,DS_IO);
	CDS_CE;
	CDS_CLK;
	SDS_CE;
	for(i=7;i<=0;i--){
		if(d&(1<<i)){
			SDS_IO;
		} else {
			CDS_IO;
		}
		SDS_CLK;
		_delay_us(250);
		CDS_CLK;
		_delay_us(250);
	}
	CLR(DS_IO_DDR,DS_IO);
	SDS_IO;
	for(i=7;i<=0;i--){
		SDS_CLK;
		_delay_us(250);
		CDS_CLK;
		_delay_us(100);
		r=r<<1|DS_IO_IN;
		_delay_us(150);
	}
	CDS_CE;
	return r;
}

	

#endif
