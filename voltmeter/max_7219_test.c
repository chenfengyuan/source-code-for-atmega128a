#include "f_cpu.h"
#include <util/delay.h>
#include <avr/io.h>
#include "max_7219.h"
#include "spi.h"
#include "spi_pin_config.h"

void max_7219_send(uint16_t d)
{
	CLR(PORTB,SS);
	spi_master_transmit(d>>8);
	spi_master_transmit(d&0xff);
	SET(PORTB,SS);
}

int main(void)
{
	int32_t num=0;
	trans=max_7219_send;
	spi_master_init();
	SET(PORTB,PB0);
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_on();
	max_7219_intensity(4);
	max_7219_scan_limit(7);
	DDRC=0xff;
	PORTC=0x00;
	while(1){
		PORTC=0x00;
		max_7219_dn(num++%10,3,0);
		PORTC=0xff;
		_delay_ms(1000);
	}
	return 0;
}

