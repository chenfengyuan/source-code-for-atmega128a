#include "adc.h"
#include "lcd12864.h"
#include "f_cpu.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "util.h"
#include "extern.h"
#include "int.h"
int32_t arr[ARR_L];
uint8_t arr2[ARR_L];
int32_t convert(int32_t v,uint8_t mode)
{
	int32_t r;
	switch(mode){
	case 0:
		r=v*0.502008;
		break;
	case 4:
		r=v*0.8880995;
		break;
	case 6:
		r=v*4.237288;
		break;
	default:
		r=v;
		break;
	}
	return r;
}
		
int main(void)
{
	uint32_t i=0;
	int32_t average=0,tmp,lowest,highest,max;
	DDRA=0xff;
	PORTA=mode;
	adc_init();
	int_init();
	lcd12864_init();
	sei();
	while(1){
		lcd12864_clear();
		lcd12864_move_cur(0,1);
		lcd12864_dis_num(mode);
		PORTA=mode;
		adc_break=0;
		average=0;
		for(i=0;i<10;++i)
			average+=adc();
		average/=i;
		lowest=average-ARR_L/2;
		highest=average+ARR_L/2;
		for(i=0;i<ARR_L;++i)
			arr[i]=0;
		for(i=0,max=1,average=0;i<420&&(adc_break==0);++i){
			tmp=adc();
			average+=tmp;
			if(tmp>=lowest&&tmp<highest){
				++arr[tmp-lowest];
				max=MAX(max,arr[tmp-lowest]);
			}
		}
		average/=i;
		for(i=0;i<ARR_L;++i){
			arr2[i]=arr[i]*64/max;
		}
		lcd12864_draw_rectangles(arr2);
		lcd12864_move_cur(0,0);
		lcd12864_dis_num(average);
		lcd12864_move_cur(0,2);
		lcd12864_dis_num(convert(average,mode));
		lcd12864_draw_pic();
		_delay_ms(1000);
	}
	return 0;
}
