#include "f_cpu.h"
#include <util/delay.h>
#include "lcd12864.h"
#include "util.h"
#include <string.h>
/* #define RS_PORT PORT(RS_PIN) */
/* #define RS_DDR DDR(RS_PIN) */
/* #define RW_PORT PORT(RW_PIN) */
/* #define RW_DDR DDR(RW_PIN) */
/* #define EN_PORT PORT(EN_PIN) */
/* #define EN_DDR DDR(EN_PIN) */

#define RS_PORT PORTD
#define RS_DDR DDRD
#define RW_PORT PORTG
#define RW_DDR DDRG
#define EN_PORT PORTG
#define EN_DDR DDRG

#define RS_1()     {RS_DDR |= (1<<RS);nop();RS_PORT |= (1<<RS);}
#define RW_1()     {RW_DDR |= (1<<RW);nop();RW_PORT |= (1<<RW);}
#define EN_1()     {EN_DDR |= (1<<EN);nop();EN_PORT |= (1<<EN);}

#define RS_0()   {RS_DDR |= (1<<RS);nop();RS_PORT &= ~(1<<RS);}
#define RW_0()   {RW_DDR |= (1<<RW);nop();RW_PORT &= ~(1<<RW);}
#define EN_0()   {EN_DDR |= (1<<EN);nop();EN_PORT &= ~(1<<EN);}

#define set_output()  LCD12864_DATA_DDR = 0XFF
#define set_input()   LCD12864_DATA_DDR = 0X00
#define nop() asm("nop")
#define uint8_t unsigned char
#define uint16_t  unsigned int

uint8_t buffer[1024] = { };

void lcd12864_busywait(void)
{
	uint8_t bf = 0;
 loop:
	RS_0();
	RW_1();
	EN_1();
	set_input();
	_delay_us(6);
	bf = LCD12864_DATA_IN;
	EN_0();
	if (bf & 0x80)
		goto loop;
	set_output();
}

void lcd12864_write_cmd(uint8_t command)
{
	lcd12864_busywait();
	set_output();
	RS_0();
	RW_0();
	LCD12864_DATA_OUT = command;
	EN_1();
	EN_0();

}

void lcd12864_write_data(uint8_t wrdata)
{
	lcd12864_busywait();
	set_output();
	RS_1();
	RW_0();
	LCD12864_DATA_OUT = wrdata;
	EN_1();
	EN_0();
}

void lcd12864_init(void)
{
	lcd12864_write_cmd(0x30);	//功能设置 8位数据，基本指令
	_delay_ms(1);
	lcd12864_write_cmd(0x0c);	//显示状态 ON，游标OFF，反白OFF
	_delay_ms(1);
	lcd12864_write_cmd(0x01);	//清除显示
	_delay_ms(1);
	lcd12864_write_cmd(0x02);	//地址归位
}

void lcd12864_move_cur(uint8_t x, uint8_t y)
{
	uint8_t addr = 0;
	switch (y) {
	case 0:
		addr = 0x80 + x;
		break;
	case 1:
		addr = 0x90 + x;
		break;
	case 2:
		addr = 0x88 + x;
		break;
	case 3:
		addr = 0x98 + x;
		break;
	default:
		break;
	}
	lcd12864_write_cmd(addr);
}

void lcd12864_dis_chinese(uint16_t c)
{
	lcd12864_write_data(c >> 8);
	lcd12864_write_data(c & 0xff);
}

static void lcd12864_draw(uint8_t * buf)
{

	uint16_t x, y;
	uint16_t k = 0;
	uint8_t y_pos = 0x80;
	lcd12864_write_cmd(0x36);	//功能设置---8BIT控制界面，扩充指令集
	for (y = 0; y < 32; y++) {
		lcd12864_write_cmd(y_pos++);	//设置绘图区的Y地址坐标
		lcd12864_write_cmd(0x80);	//设置绘图区的X地址坐标
		for (x = 0; x < 8; x++) {
			lcd12864_write_data(buf[k++]);
			lcd12864_write_data(buf[k++]);
		}
	}
	y_pos = 0x80;
	for (y = 0; y < 32; y++) {
		lcd12864_write_cmd(y_pos++);	//设置绘图区的Y地址坐标
		lcd12864_write_cmd(0x88);	//设置绘图区的X地址坐标
		for (x = 0; x < 8; x++) {
			lcd12864_write_data(buf[k++]);
			lcd12864_write_data(buf[k++]);
		}
	}
	lcd12864_write_cmd(0x30);	//功能设置 8位数据，基本指令
	lcd12864_write_cmd(0x0c);	//显示状态 ON，游标OFF，反白OFF
	lcd12864_write_cmd(0x02);	//地址归位
}

/* 1<=x<=128 ,1<=y<=64*/
static void plot_12864(int x, int y, uint8_t * buf)
{
	uint8_t a = 0;
	if (!(x % 8)) {
		a |= (1 << 0);
		buf[(64 - y) * 16 + (x / 8) - 1] |= a;
	} else {
		a |= (1 << (8 - x % 8));
		buf[(64 - y) * 16 + (x / 8)] |= a;
	}

}

static void draw_rectangle(uint8_t n, uint8_t h, uint8_t * buf)
{
	uint8_t i, j;
	for (i = n * 2 + 1; i <= n * 2 + 2; ++i) {
		for (j = 1; j <= h; ++j) {
			plot_12864(i, j, buf);
		}
	}
}

void lcd12864_draw_rectangles(uint8_t arr[])
{
	int i;
	for (i = 0; i < 64; ++i) {
		draw_rectangle(i, arr[i], buffer);
	}
}

void lcd12864_draw_pic(void)
{
	lcd12864_draw(buffer);
}

void lcd12864_dis_num(int32_t num)
{
	unsigned char arr[10];
	int i;
	if (num == 0) {
		lcd12864_write_data('0');
	} else {

		for (i = 0; num > 0; ++i) {
			arr[i] = num % 10;
			num /= 10;
		}
		for (i -= 1; i >= 0; --i)
			lcd12864_write_data(arr[i] + '0');
	}
}

void lcd12864_clear(void)
{
	lcd12864_write_cmd(0x01);
	memset(buffer, 0, sizeof(buffer));
}

void lcd12864_dis_str(char *str)
{
	while (*str) {
		lcd12864_write_data(*str++);
	}
}
