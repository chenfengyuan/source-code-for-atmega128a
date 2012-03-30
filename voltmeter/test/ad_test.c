#include "../f_cpu.h"
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../max_7219.h"
#include "../spi.h"
#include "../spi_pin_config.h"
#include "lcd12864.c"

void max_7219_send(uint16_t d)
{
	CLR(PORTB,SS);
	spi_master_transmit(d>>8);
	spi_master_transmit(d&0xff);
	SET(PORTB,SS);
}

ISR(INT0_vect)
{
	/* PORTC=0xff; */
	/* sleep_disable(); */
}

ISR(ADC_vect)
{
	/* PORTC=~PORTC; */
	/* unsigned char l,h; */
	/* sleep_disable(); */
	/* l=ADCL; */
	/* h=ADCH; */
	/* max_7219_number(h<<8|l); */
	/* _delay_ms(1000); */
	/* set_sleep_mode(SLEEP_MODE_ADC); */
	/* sleep_enable(); */
	/* sei(); */
	/* sleep_cpu(); */
}

int main(void)
{
	unsigned char l,h;
	int i;
	int max,max_i;
	int arr[64];
	DDRF=0x00;
	PORTF=0x00;
	DDRA=0xff;
	PORTA=0xff;
	
	/* DDRC=0xff; */
	/* PORTC=0xff; */
	trans=max_7219_send;
	spi_master_init();
	SET(PORTB,PB0);
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_on();
	max_7219_intensity(4);
	ADMUX=0xc0;
	/* max_7219_scan_limit(7); */
	/* ADMUX=(1<<REFS1)|(1<<REFS0); */
	ADCSRA=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	set_sleep_mode(SLEEP_MODE_ADC);
	sei();
	/* while(1){ */
	/* 	sleep_enable(); */
	/* 	sleep_cpu(); */
	/* 	sleep_disable(); */
	/* 	l=ADCL; */
	/* 	h=ADCH; */
	/* 	max_7219_number(h<<8|l); */
	/* 	_delay_ms(1000); */
	/* } */
	Init_12864();
	while(1){
		for(i=0;i<64;++i)
			arr[i]=0;
		for(i=0;i<10000;++i){
			sleep_enable();
			sleep_cpu();
			sleep_disable();
			l=ADCL;
			h=ADCH;
			++arr[(h<<8|l)-840];
		}
		max=0;
		for(i=0;i<64;++i){
			if(arr[i]>max){
				max=arr[i];
				max_i=i;
			}
		}
		max_7219_number(max);
		for(i=0;i<64;++i){
			arr[i]=arr[i]*64.0/max;
		}
		for(i=0;i<1024;++i){
			music[i]=0;
		}
		draw_rectangles(arr);
		drawpic_12864(music);
		PORTA=~PORTA;
		_delay_ms(500);
	}
	
	/* while(1); */
	/* while(1){ */
	/* 	sleep_enable(); */
	/* 	sleep_cpu(); */
	/* 	sleep_disable(); */
	/* 	l=ADCL; */
	/* 	h=ADCH; */
	/* 	max_7219_number((h<<8|l)); */
	/* 	_delay_ms(1000); */
	/* } */
	

	/* while(1){ */
	/* 	ADCSRA=0xc7; */
	/* 	while(!(ADCSRA&(1<<ADIF))); */
	/* 	l=ADCL; */
	/* 	h=ADCH; */
	/* 	max_7219_number(h<<8|l); */
	/* 	ADCSRA|=(1<<ADIF); */
	/* 	_delay_ms(1000); */
	/* } */
	/* /\* _delay_ms(1000); *\/ */
	/* /\* set_sleep_mode(SLEEP_MODE_ADC); *\/ */
	/* /\* sleep_enable(); *\/ */
	/* /\* sei(); *\/ */
	/* /\* sleep_cpu(); *\/ */
	/* /\* sleep_disable(); *\/ */
	/* return 0; */

	/* EIMSK=1<<INT0; */
	/* EICRA=(1<<ISC01)||(1<<ISC00); */
	/* set_sleep_mode(SLEEP_MODE_ADC); */
	/* sleep_enable(); */
	/* sei(); */
	/* sleep_cpu(); */
	/* sleep_disable(); */
	/* while(1){ */
	/* 	asm volatile("com r24\n\t" */
	/* 		     "out 0x15,r24\n\t" */
	/* 		); */
	/* } */
	/* while(1); */
	
	return 0;
}

