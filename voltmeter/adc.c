#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "adc.h"
ISR(ADC_vect)
{
}

void adc_init(void)
{
	ADMUX=ADMUX_INIT;
	ADCSRA=ADCSRA_INIT;
	set_sleep_mode(SLEEP_MODE_ADC);
}

uint16_t adc(void)
{
	uint8_t l,h;
	sleep_enable();
	sleep_cpu();
	sleep_disable();
	l=ADCL;
	h=ADCH;
	return (h<<8|l);
}
