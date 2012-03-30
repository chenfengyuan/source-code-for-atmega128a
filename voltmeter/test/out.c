#include "../f_cpu.h"
#include <util/delay.h>
#include <avr/io.h>

int main(void)
{
	DDRC=0xff;
	PORTC=0xff;
	while(1){
		_delay_ms(3000);
		PORTC=~PORTC;
	}
	
	return 0;
}

