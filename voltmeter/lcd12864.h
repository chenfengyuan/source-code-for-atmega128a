#ifndef _LCD_12864_H
#define _LCD_12864_H 1
#include <avr/io.h>
#include <math.h>
#define LCD12864_DATA_OUT PORTC
#define LCD12864_DATA_IN  PINC
#define LCD12864_DATA_DDR DDRC
#define RS_PIN D
#define RS 7
#define RW_PIN G
#define RW 0
#define EN_PIN G
#define EN 1
#define ARR_L 64
void lcd12864_dis_num(int32_t num);
void lcd12864_init(void);
void lcd12864_move_cur(uint8_t x,uint8_t y);
void lcd12864_write_cmd(uint8_t command);
void lcd12864_write_data(uint8_t wrdata);
void lcd12864_draw_rectangles(uint8_t arr[]);
void lcd12864_draw_pic(void);
void lcd12864_clear(void);
#endif
