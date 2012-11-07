#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h> 
#include <avr/interrupt.h>

int main(void)
{
	DDRB=0xff;
	TCCR0=(1<<WGM00)|(1<<COM01)|(1<<COM00)|(1<<CS0)|(1<<CS01);
	sei();
	while(1){
		OCR0=0xff>>1;
		_delay_ms(3000);
		OCR0=0xaf;
		_delay_ms(3000);
	}
	return 0;
}

	
