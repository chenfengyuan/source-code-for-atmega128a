#include "adc.h"
#include "lcd12864.h"
#include "f_cpu.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "util.h"
#include "extern.h"
#include "int.h"
#include <stdio.h>
int32_t arr[ARR_L];
char str_temp[10]="aoeu";
int32_t convert(int32_t v,uint8_t mode)
{
	double a,b;
	switch(mode){
	case 0:
		a=0.530025;
		b=-26.6847;
		break;
	case 1:
		a=0.980935;
		b=-48.8349;
		break;
	case 2:
		if(v<=194){
			double a,b,c,d,e,x;
			a = -6.66392e-07;
			b = 0.000360131;
			c = -0.0896023;
			d = 28.7339;
			e = -1272.24;
			x=v;
			return e+x*(d+x*(c+x*(b+x*a)));
		} else {
			double a,b,c,d,e,x;
			a = 4.8329e-08;
			b = -8.83833e-05;
			c = 0.041316;
			d = 13.1324;
			e = -999.192;
			x=v;
			return e+x*(d+x*(c+x*(b+x*a)));
		}
		break;
	case 3:
		a=0.10825;
		b=-5.20943;
		break;
	case 5:
		a=7.19968;
		b=-349.696;
		break;
	default:
		return v;
		break;
	}
	return (int32_t)(a*v+b);
}
/*                    0            1           2         3             4            5 */
char *mode_str[]={"0~500 毫伏","0~1000毫伏","0~10  伏","0~100 毫伏","not such mode","0~5   伏"};
void init(void)
{
	DDRA=0xff;
	adc_init();
	int_init();
	lcd12864_init();
	sei();
	lcd12864_clear();
	lcd12864_move_cur(0,0);
	lcd12864_dis_str(mode_str[mode]);
}
uint8_t below(int32_t reality,uint8_t mode)
{
	switch(mode){
	case 0:
		return reality<=500;
	case 1:
		return reality<=10000;
	case 2:
		return reality<=10000;
	case 3:
		return reality<=100;
	case 5:
		return reality<=5000;
	default:
		return 0;
	}
}

void dis(int32_t average,uint8_t mode)
{
	int32_t reality;
	lcd12864_clear();
	lcd12864_move_cur(0,0);
	lcd12864_dis_str("多量程直流电压表");
	lcd12864_move_cur(0,1);
	lcd12864_dis_str("挡位:");
	lcd12864_move_cur(3,1);
	lcd12864_dis_str(mode_str[mode]);
	lcd12864_move_cur(0,2);
	lcd12864_dis_str("电压:");
	reality=convert(average,mode);
	if(below(reality,mode)){
		if(mode==2||mode==5){
			sprintf(str_temp,"%ld.%ld",reality/1000,reality%1000);
			lcd12864_move_cur(6,2);
			lcd12864_dis_str("伏");
			lcd12864_move_cur(3,2);
			lcd12864_dis_str(str_temp);
		} else {
			lcd12864_move_cur(3,2);
			lcd12864_dis_num(reality);
			lcd12864_move_cur(6,2);
			lcd12864_dis_str("毫伏");
		}
	} else {
		lcd12864_move_cur(3,2);
		lcd12864_dis_str("超量程");
	}
	lcd12864_move_cur(0,3);
	lcd12864_dis_num(average);
}
int32_t fast_measure(uint8_t times)
{
	uint8_t i=0;
	int32_t average=0;
	PORTA=2;
	for(i=0;i<times;++i){
		average+=adc();
	}
	return average/=i;
}

uint8_t best_mode(int32_t average)
{
	int32_t reality=convert(average,2);
	if(reality<=80)
		return 3;
	else if(reality<=400)
		return 0;
	else if(reality<=800)
		return 1;
	else if(reality<=4000)
		return 5;
	else
		return 2;
}


int32_t measure(uint8_t mode)
{
	uint32_t i=0;
	int32_t average=0,tmp,lowest,highest,max;
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
	return average;
}

int main(void)
{
	init();
	for(;;){
		if(mode!=42){
			dis(measure(mode),mode);
			lcd12864_move_cur(3,3);
			lcd12864_dis_str("手动");
		} else {
			uint8_t mode=best_mode(fast_measure(10));
			dis(measure(mode),mode);
			lcd12864_move_cur(3,3);
			lcd12864_dis_str("自动");
		}
		_delay_ms(1000);
	}
	return 0;
}
