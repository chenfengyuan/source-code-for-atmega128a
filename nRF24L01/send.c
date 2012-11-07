#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

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

/* #define W2_CSN_INIT SET(DDRC,PC0) */
/* #define W2_CSN_H SET(PORTC,PC0) */
/* #define W2_CSN_L CLR(PORTC,PC0) */
/* #define W2_CE_INIT SET(DDRC,PC1) */
/* #define W2_CE_H SET(PORTC,PC1) */
/* #define W2_CE_L CLR(PORTC,PC1) */

#define W1_CSN_INIT SET(DDRC,PC2)
#define W1_CSN_H SET(PORTC,PC2)
#define W1_CSN_L CLR(PORTC,PC2)
#define W1_CE_INIT SET(DDRC,PC3)
#define W1_CE_H SET(PORTC,PC3)
#define W1_CE_L CLR(PORTC,PC3)

int main(void)
{
	char n=0,i;
	/* PIN initial */
	SPI_MasterInit();
	/* max_7219_init(); */
	W1_CSN_INIT;
	W1_CE_INIT;
	/* max_7219_test_off(); */
	/* max_7219_clear(); */
	/* max_7219_on(); */
	/* max_7219_decode_all_off(); */
	/* _max_7219_intensity(5); */
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
	W1_CE_L;
	W1_CSN_H;

	/* change address */
	/* W1_CSN_L; */
	/* SPI_MasterTransmit((1<<5)|(0x10)); */
	/* SPI_MasterTransmit(0x1); */
	/* W1_CSN_H; */

	/* W1_CSN_L; */
	/* SPI_MasterTransmit((1<<5)+7); */
	/* SPI_MasterTransmit(0xe); */
	/* W1_CSN_H; */

	W1_CSN_L;
	SPI_MasterTransmit(1<<5);
	SPI_MasterTransmit((1<<3)|(1<<1));
	W1_CSN_H;

	W1_CE_H;
	_delay_ms(100);
	W1_CE_L;
	
	W1_CSN_L;
	SPI_MasterTransmit(0xa0);
	SPI_MasterTransmit(0xe);
	W1_CSN_H;

	/* W1_CSN_L; */
	/* SPI_MasterTransmit(0xe1); */
	/* W1_CSN_H; */

	/* W1_CE_L; */
	/* _delay_ms(10); */
	/* W1_CE_H; */
	

	/* _delay_ms(100); */
	/* W1_CSN_L; */
	/* SPI_MasterTransmit(0x7); */
	/* SPI_MasterTransmit(0xff); */
	/* W1_CSN_H; */
	/* n=SPI_SlaveReceive(); */
	/* max_7219_hex_number(n); */
	
	while(1)
		;
	return 0;
}

	
