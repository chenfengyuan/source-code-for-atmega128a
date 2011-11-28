#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "spi.h"
#define CS_PIN G
#define CS 3
#define DIN_PIN G
#define DIN 4
#define CLK_PIN G
#define CLK 2
#include "../max_7219/max_7219.h"



void SPI_SlaveInit(void)
{
/* Set MISO output, all others input */
	DDR_SPI = (1<<DD_MISO);
/* Enable SPI */
	SPCR = (1<<SPE);
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
	char i=0;
	max_7219_init();
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_on();
	_max_7219_intensity(2);
	SPI_SlaveInit();
	while(1){
		SPDR=i+5;
		i=SPI_SlaveReceive();
		max_7219_number(i);
	}
	return 0;
}
