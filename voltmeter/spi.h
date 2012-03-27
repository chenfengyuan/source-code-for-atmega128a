#ifndef _SPI_H
#define _SPI_H 1
void spi_master_init(void);
void spi_master_transmit(char data);
char spi_slave_receive(void);
#endif

