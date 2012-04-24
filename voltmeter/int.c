#include <avr/interrupt.h>
#include "int.h"
#include "extern.h"
#include "util.h"
#include "f_cpu.h"
#include <util/delay.h>
ISR(INT0_vect)
{
	cli();
	_delay_ms(50);
	if((PIND&1)==0){
		mode=(mode+1)%8;
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


