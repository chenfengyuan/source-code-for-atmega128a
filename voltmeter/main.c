#include "adc.h"
#include "lcd12864.h"
#include "f_cpu.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "util.h"
uint32_t arr[ARR_L];
uint8_t arr2[ARR_L];
int main(void)
{
	uint32_t i=0;
	uint32_t average=0,tmp,lowest,highest,max;
	adc_init();
	lcd12864_init();
	sei();
	while(1){
		average=0;
		for(i=0;i<10;++i)
			average+=adc();
		average/=10;
		lowest=average-ARR_L/2;
		highest=average+ARR_L/2;
		for(i=0;i<ARR_L;++i)
			arr[i]=0;
		for(i=0,max=1;i<10000;++i){
			tmp=adc();
			if(tmp>=lowest&&tmp<highest){
				++arr[tmp-lowest];
				max=MAX(max,arr[tmp-lowest]);
			}
		}
		for(i=0;i<ARR_L;++i){
			arr2[i]=arr[i]*64/max;
		}
		lcd12864_clear();
		lcd12864_draw_rectangles(arr2);
		lcd12864_dis_num(average);
		lcd12864_draw_pic();
		_delay_ms(3000);
	}
	return 0;
}
