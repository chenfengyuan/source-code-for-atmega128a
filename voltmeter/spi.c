#include <avr/io.h>
#include "spi_pin_config.h"

void spi_master_init(void)
{
/* Set MOSI and SCK output, all others input */
	DDR_SPI = 1 << DD_MOSI | 1 << DD_SCK | 1 << SS;

/* Enable SPI, Master, set clock rate fck/2 */
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR0);
	SPSR = (1 << SPI2X);
}

void spi_master_transmit(char data)
{
/* Start transmission */
	SPDR = data;
/* Wait for transmission complete */
	while (!(SPSR & (1 << SPIF))) ;
}

char spi_slave_receive(void)
{
/* Wait for reception complete */
	while (!(SPSR & (1 << SPIF))) ;
/* Return data register */
	return SPDR;
}
