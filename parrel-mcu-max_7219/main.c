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

unsigned char freq[4]={0xff,0xff,0xff,0xff};
unsigned char activity[4]={0xff,0xff,0xff,0xff};
char toggle=0;

void port_init()
{
	/* pin a is the address,ouput */
	DDRA=0xff;
	PORTA=0xff;
	/* pin c is the data,input */
	DDRC=0;
	PORTC=0;
}

ISR(TIMER1_COMPA_vect)
{
	uint32_t num=0;
	unsigned char * a;
	char dot=0;
	int i;

	if(toggle==0){
		a=freq;
		toggle=1;
		dot=0;
		for(i=3;i>=1;i--){
			num|=a[i];
			num<<=8;
		}
		num|=a[0];
	} else {
		a=activity;
		toggle=0;
		dot=1;
		for(i=3;i>=1;i--){
			num|=a[i];
			num<<=8;
		}
		num|=a[0];
		num=(uint32_t)(num/300000000.0*100000000);
		if(num%10>=5){
			num+=10;
		}
		num/=10;
	}
	for(i=0;i<8;i++){
		_max_7219_dn(num%10,i,dot&&(i==7));
		num/=10;
	}
	/* int i; */
	/* unsigned char * a= activity; */
	
	/* for(i=3;i>=0;i--){ */
	/* 	max_7219_hex_and_shift_left((a[i]>>4)&0xf); */
	/* 	max_7219_hex_and_shift_left(a[i]&0xf); */
	/* } */
}
		
int main(void)
{
	int i;
	port_init();
	/* time 1 configure */
	TCCR1A=(1<<WGM10);
	TCCR1B=(1<<CS12)|(1<<CS10)|(1<<WGM13);
	OCR1A=15624;
	TIMSK=1<<OCIE1A;
	
	/* PIN initial */
	max_7219_init();
	max_7219_test_off();
	max_7219_clear();
	_max_7219_scan_limit(7);
	max_7219_on();
	max_7219_decode_all_on();
	_max_7219_intensity(5);
	sei();
	while(1){
		for(i=0;i<4;i++){
			PORTA=i;
			_delay_ms(1);
			freq[i]=PINC;
		}
		for(i=0;i<4;i++){
			PORTA=(1<<3)|i;
			_delay_ms(1);
			activity[i]=PINC;
		}
	}
	return 0;
}

	
		
