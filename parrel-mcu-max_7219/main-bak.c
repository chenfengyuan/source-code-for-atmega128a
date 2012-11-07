#define F_CPU 16000000UL  /* 16 MHz */
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define DIN_PIN G
#define DIN 4
#define CS_PIN G
#define CS 3
#define CLK_PIN G
#define CLK 2


#include "../max_7219/max_7219.h"

char a[4]={0xff,0xff,0xff,0xff};
void port_init()
{
	DDRA=0;
	PORTA=0;
	DDRC=0;
	PORTC=0;
	/* PE7 is rising edge when the data is available,input*/
	CLR(DDRE,PE7);
	CLR(PORTE,PE7);
	/* PC2 is high while the mcu is reading,output*/
	SET(DDRC,PC0);
	SET(PORTC,PC0);
	/* PAn,PBn,PCn,PDn is the data input pin */
}

ISR(INT7_vect)
{
	cli();
	SET(PORTC,PC2);
	a[PINC & 0x3]=PINA;
	CLR(PORTC,PC2);
	sei();
}
ISR(TIMER1_COMPA_vect)
{
	cli();
	int i;
	for(i=3;i>=0;i--){
		max_7219_hex_and_shift_left(a[i]>>4);
		max_7219_hex_and_shift_left(a[i]&0xf);
	}
	sei();
}
		
int main(void)
{
	port_init();
	/* external interrupt 7 configure */
	EICRB=(1<<ISC71)|(1<<ISC70);
	EIMSK=(1<<INT7);
	/* time 1 configure */
	TCCR1A=(1<<WGM10);
	TCCR1B=(1<<CS12)|(1<<CS10)|(1<<WGM13);
	OCR1A=15624;
	TIMSK=1<<OCIE1A;
	
	/* PIN initial */
	max_7219_init();
	max_7219_test_off();
	max_7219_clear();
	max_7219_on();
	max_7219_decode_all_off();
	_max_7219_intensity(5);
	sei();
	while(1)
		;
	return 0;
}

	
		
