#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#define CLR(BYTE,BIT) ((BYTE)&=~(1<<(BIT)))
#define SET(BYTE,BIT) ((BYTE)|=(1<<(BIT)))

/* spi setting */
#define DDR_SPI DDRB
#define DD_MISO PB3
#define DD_MOSI PB2
#define DD_SCK PB1
#define SS PB0

/* max 7219 setting */
#define DIN_PIN A
#define DIN 0
#define CS_PIN A
#define CS 1
#define CLK_PIN A
#define CLK 2
#include "../max_7219/max_7219.h"

void SPI_MasterInit(void)
{
/* Set MOSI and SCK output, all others input */
	DDR_SPI = 1<<DD_MOSI|1<<DD_SCK|1<<SS;
	
/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
/* Start transmission */
	SPDR = cData;
/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
		;
}

char SPI_SlaveReceive(void)
{
/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
		;
/* Return data register */
	return SPDR;
}

#define W1_CSN_INIT SET(DDRB,PB4)
#define W1_CSN_H SET(PORTB,PB4)
#define W1_CSN_L CLR(PORTB,PB4)
#define W1_CE_INIT SET(DDRB,PB5)
#define W1_CE_H SET(PORTB,PB5)
#define W1_CE_L CLR(PORTB,PB5)

int main(void)
{
	char n=0,i;
	/* PIN initial */
	SPI_MasterInit();
	W1_CSN_INIT;
	W1_CE_INIT;
	DDRC=0xff;
	PORTC=0xff;
	max_7219_init();
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_off();
	/* max_7219_hex_number(0x42); */
	_delay_ms(42);
	/* set prim_rx low */
	/* SPI_MasterTransmit(1<<5); */
	/* SPI_MasterTransmit(0xb); */
	/* while(1){ */
	/* 	SPI_MasterTransmit(9); */
	/* 	n=SPI_SlaveReceive(); */
	/* 	max_7219_hex_number(n); */
	/* 	_delay_ms(1000); */
	/* } */
	/* set w2 in receive mode */
	/* stand by mode */
	W1_CE_L;
	W1_CSN_H;

	/* recevie bytes in data pipe 0 */
	/* W1_CSN_L; */
	/* SPI_MasterTransmit(1<<5|0x11); */
	/* SPI_MasterTransmit(1); */
	/* W1_CSN_H; */

	/* power up */
	W1_CSN_L;
	SPI_MasterTransmit(1<<5);
	SPI_MasterTransmit((1<<3)|(1<<1)|(1<<0));
	W1_CSN_H;

	W1_CE_H;
	PORTC=0xff;
	n=0;
	while(1){
		W1_CSN_L;
		SPI_MasterTransmit(0x61);
		SPI_MasterTransmit(0xff);
		W1_CSN_H;
		n=SPI_SlaveReceive();
		if(n!=0){
			max_7219_hex_number(n);
			break;
		}
		
		/* W1_CSN_L; */
		/* SPI_MasterTransmit(0x9); */
		/* SPI_MasterTransmit(0xff); */
		/* W1_CSN_H; */
		/* n=SPI_SlaveReceive(); */
		/* if(n==1){ */
		/* 	PORTC=0; */
		/* 	break; */
		/* } */
		PORTC=(~PORTC)|1;
	}
	while(1);
	
	return 0;
	
}

	
