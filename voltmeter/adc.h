#ifndef _ADC_H
#define _ADC_H 1
#include <stdint.h>
#define ADMUX_INIT (0xc0|7)
#define ADCSRA_INIT ((1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0))
void adc_init(void);
uint16_t adc(void);
#endif

