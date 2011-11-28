#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "spi.h"

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

int main(void)
{
	char i=1;
	SPI_MasterInit();
	while(1){
		SPI_MasterTransmit(i);
		i=SPI_SlaveReceive();
		_delay_ms(1000);
	}
	return 0;
}
