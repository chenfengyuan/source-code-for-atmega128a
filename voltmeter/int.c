#include <avr/interrupt.h>
#include "int.h"
#include "extern.h"
#include "util.h"
#include "f_cpu.h"
#include <util/delay.h>
uint8_t mode_switch(uint8_t pre)
{
	uint8_t mode;
	switch(pre){
	case 0:
		mode=1;
		break;
	case 1:
		mode=2;
		break;
	case 2:
		mode=3;
		break;
	case 3:
		mode=5;
		break;
	case 5:
		mode=42;
		break;
	case 42:
		mode=0;
		break;
	default:
		mode=2;
	}
	return mode;
}

ISR(INT0_vect)
{
	cli();
	_delay_ms(50);
	if((PIND&1)==0){
		mode=mode_switch(mode);
	}
	adc_break=1;
	sei();
}
void int_init(void)
{
	EICRA=EICRA_INIT;
	EIMSK=EIMSK_INIT;
	CLR(DDRD,PA0);
	SET(PORTD,PA0);
}


