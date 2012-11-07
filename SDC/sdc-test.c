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
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);
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

int main(void)
{
	unsigned char n=0;
	char i;
	/* PIN initial */
	SPI_MasterInit();
	max_7219_init();
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_off();
	_max_7219_intensity(5);
	_delay_ms(42);
	max_7219_hex_number(42);
	while(!(PINB&(1<<PB3)))
		;
	SPI_MasterTransmit((1<<6)|0);
	for(i=0;i<4;i++){
		SPI_MasterTransmit(0);
	}
	SPI_MasterTransmit(1);
	/* SPI_MasterTransmit((1<<6)|1); */
	/* for(i=0;i<4;i++){ */
	/* 	SPI_MasterTransmit(0); */
	/* } */
	/* SPI_MasterTransmit(1); */
	/* SPI_MasterTransmit((1<<6)|10); */
	/* for(i=0;i<4;i++){ */
	/* 	SPI_MasterTransmit(0); */
	/* } */
	/* SPI_MasterTransmit(1); */
	/* n=0; */
	while(!(PINB&(1<<PB3)))
		;
	SPI_MasterTransmit(0xff);
	n=SPI_SlaveReceive();
	for(i=7;i>=0;i--){
		max_7219_hex_and_shift_left(n&(1<<i)?1:0);
	}
	while(1);
}
