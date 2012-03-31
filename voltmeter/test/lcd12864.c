#include "../f_cpu.h"
#include <avr/io.h>
#include <util/delay.h>  
#include <math.h>

#define data_port    PORTC
#define RS_1()     {DDRD |= (1<<PD7);nop();PORTD |= (1<<PD7);}
#define RW_1()     {DDRG |= (1<<PG0);nop();PORTG |= (1<<PG0);}
#define EN_1()     {DDRG |= (1<<PG1);nop();PORTG |= (1<<PG1);}

#define RS_0()   {DDRD |= (1<<PD7);nop();PORTD &= ~(1<<PD7);}
#define RW_0()   {DDRG |= (1<<PG0);nop();PORTG &= ~(1<<PG0);}
#define EN_0()   {DDRG |= (1<<PG1);nop();PORTG &= ~(1<<PG1);}

#define set_output()  DDRC = 0XFF
#define set_input()   DDRC = 0X00
#define port_lcd   PINC
#define nop() asm("nop")
#define uchar unsigned char
#define uint  unsigned int

uchar   music[1024]={};
uchar   magic[16]={};

//**********************************************************************
void delay_100ms(uint i)
{
	uint k;
	for (k=0;k<i;k++)
	{
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
	}
	
}

//**********************************************************************


//*************函数部分***************/
/* 名字：busywait_12864()            */
/* 功能：检测RT12864M是否忙          */
/* 局部变量：无                      */
/* 全局变量：无                      */
/* 返回值：  无                      */
//************************************/
void busywait_12864(void)
{
	uchar bf=0;
	
loop:RS_0();
	RW_1();
	EN_1();
	set_input();
	_delay_us(6);
	bf = port_lcd;
	EN_0();
	if(bf&0x80)goto loop;
	set_output();
}
//*************函数部分***************/
/* 名字：writecomm_12864()           */
/* 功能：写命令进RT12864M            */
/* 局部变量：command                 */
/* 全局变量：无                      */
/* 返回值：  无                      */
//************************************/
void writecomm_12864(uchar command)
{
	busywait_12864();
	set_output();
	RS_0();
	RW_0();
	data_port = command;
	EN_1();
	EN_0();

}
//*************函数部分***************/
/* 名字：writedata_12864()           */
/* 功能：写数据写进RT12864M          */
/* 局部变量：wrdata                  */
/* 全局变量：无                      */
/* 返回值：  无                      */
//************************************/
void writedata_12864(uchar wrdata)
{
	busywait_12864();
	set_output();
	RS_1();
	RW_0();
	data_port = wrdata;
	EN_1();
	EN_0();
}
//*************函数部分***************/
/* 名字：Init_12864()                */
/* 功能：初始化RT12864M              */
/* 局部变量：无                      */
/* 全局变量：无                      */
/* 返回值：  无                      */
//************************************/
void Init_12864(void)
{
	writecomm_12864(0x30);//功能设置 8位数据，基本指令
	_delay_ms(1);
	writecomm_12864(0x0c);//显示状态 ON，游标OFF，反白OFF
	_delay_ms(1);
	writecomm_12864(0x01);//清除显示
	_delay_ms(1);
	writecomm_12864(0x02);//地址归位
}
//*************函数部分***************/
/* 名字：locatexy_12864()            */
/* 功能：定位显示地址                */
/* 局部变量：x,y                     */
/* 全局变量：无                      */
/* 返回值：  无                      */
//************************************/
void locatexy_12864(uchar x,uchar y)
{
	uchar addr=0;
	switch(y)
        {
	case 0:addr=0x80+x;break;
	case 1:addr=0x90+x;break;
	case 2:addr=0x88+x;break;
	case 3:addr=0x98+x;break;
	default:break;
	}	
	writecomm_12864(addr);
}
//*************函数部分***************/
/* 名字：distwochar_12864()          */
/* 功能：显示两个字符即一个汉字      */
/* 局部变量：x,y，char1,char2        */
/* 全局变量：无                      */
/* 返回值：  无                      */
//************************************/
void distwochar_12864(uchar x,uchar y,uchar char1,uchar char2)
{
	locatexy_12864(x,y);
	writedata_12864(char1);
	writedata_12864(char2);
}
//*************函数部分***************/
/* 名字：dispstr_12864()             */
/* 功能：显示汉字字组                */
/* 局部变量：x,y，*pstr              */
/* 全局变量：无                      */
/* 返回值：  无                      */
//************************************/
void dispstr_12864(uchar x,uchar y,uchar *pstr)
{
	uint i,t=0;
	while(pstr[t]!='\0')t++;
	for(i=0;i<t;i++)
        {
		distwochar_12864(x,y,pstr[i],pstr[i+1]);
		i+=1;
		x+=1;
		if(x==8)
		{
			x=0;
			y++;
			if(y==4)y=0;
		}
	}
}

/*=========================================*/
/* 函数：drawpic(uchar *pdraw)；            */
/* 功能描述：画图形                        */
/* 参数接口：pdraw，是存放图形数据的表     */
/*=========================================*/
void drawpic_12864(uchar *pdraw)
{
	uint x,y;
	long uint k=0;
	uchar yaddr=0x80;
	//writecomm_12864(0x30);//功能设置---8BIT控制界面，绘图显示OFF
	writecomm_12864(0x36);//功能设置---8BIT控制界面，扩充指令集
	for(y=0;y<32;y++)
	{
	        writecomm_12864(yaddr++);//设置绘图区的Y地址坐标
	        writecomm_12864(0x80);  //设置绘图区的X地址坐标
	        for(x=0;x<8;x++)
		{
			writedata_12864(pdraw[k++]);
			writedata_12864(pdraw[k++]);
		}
	}
	yaddr=0x80;
	for(y=0;y<32;y++)
	{	
	        writecomm_12864(yaddr++);//设置绘图区的Y地址坐标
	        writecomm_12864(0x88);   //设置绘图区的X地址坐标
	        for(x=0;x<8;x++)
		{
			writedata_12864(pdraw[k++]);
			writedata_12864(pdraw[k++]);
		}
	}
	// writecomm_12864(0x32);//功能设置---8BIT控制界面，绘图显示ON
	writecomm_12864(0x36);//功能设置---8BIT控制界面，绘图显示OFF
	//返回基本设置
	
	writecomm_12864(0x30);//功能设置 8位数据，基本指令
	writecomm_12864(0x0c);//显示状态 ON，游标OFF，反白OFF
	writecomm_12864(0x02);//地址归位
}



//*************函数部分***************************************************
// 名字：dispstr_12864()
// 功能：移动显示汉字字组
// 局部变量：x,y12,*pstr1,*pstr2,word_number,speed
// 使用说明：x为文字定位地址横坐标?
//           y1为第（1~4）行文字定位地址坐标
//           y2为第（1~4）行文字定位地址坐标
//           *pstr1为第一行文字组
//           *pstr2为第二行文字组
//           word_number 移动显示的文字数目
//           speed 移动的速度
// 全局变量：无
// 返回值：  无
//***********************************************************************
void dispstr_12864_move(uchar x,uchar y1,uchar y2,
			uchar *pstr1,
			uint word_number,uint speed)
{
	uchar i,t;
	uint move;
	

	for (move=0;move<word_number;move++)
	{
		for (t=0;t<16;t++)
		{
	
			for(i=0;i<t;i++)
			{
				distwochar_12864(x,y1,pstr1[i+move*2],pstr1[i+move*2+1]);
				//distwochar_12864(x,y2,pstr2[i+move*2],pstr2[i+move*2+1]);
				i+=1;
				x+=1;
				if(x==8)
				{
					x=0;
				}
			}
		}
		delay_100ms(speed);
		
	}

}


//******************************************************
//*************函数部分********************************/
/* 名字：abs(x1-x0)                                  */
/* 功能：计算两个数相减的绝对值                       */
/* 局部变量：取值范围                                 */
/*                                                    */
/* 全局变量：无                                       */
/* 返回值：  无                                       */
/* 注意                                               */
//*****************************************************/
uint abs_sub(int a,int b)
{
	uint fabs = 0;
	if ((a-b)>0)   fabs = a-b;
	else           fabs = b-a;
	return (fabs);
}
//****************************************************
//*************函数部分****************************/
/* 名字：plot1_12864()                            */
/* 功能：图象单点描点函数                         */
/* 局部变量：取值范围  x：[-64，64]  y:[-32,31]   */
/* 全局变量：无                                   */
/* 返回值：  无                                   */
//*************************************************/
void plot1_12864(int x,int y,uchar *buf)
{
	uchar a=0;
	if ((x>0)|(x==0))
	{
		if (!((x+64)%8))
		{
			a|=(1<<0);
			buf[(31-y)*16+((x+64)/8)-1]|=a;
		}
		else
		{
			a|=(1<<(8-(x+63)%8));
			buf[(31-y)*16+((x+64)/8)]|=a;
		}
	}
	else
	{
		
		if (!((64+x)%8))
		{
			a|=(1<<0);
			buf[(31-y)*16+((64+x)/8)-1]|=a;
		}
		else
		{
			a|=(1<<(8-(x+64)%8));
			buf[(31-y)*16+(64+x)/8]|=a;
		}
	}

}


//*************函数部分****************************/
/* 名字：plot1_12864()                            */
/* 功能：图象单点描点函数                         */
/* 局部变量：取值范围  x：[-64，64]  y:[-32,31]   */
/* 全局变量：无                                   */
/* 返回值：  无                                   */
//*************************************************/
void plot_12864(int x,int y,uchar *buf)
{
	uchar a=0;
	
	if (!(x%8))
	{
		a|=(1<<0);
		buf[(64-y)*16+(x/8)-1]|=a;
	}
	else
	{
		a|=(1<<(8-x%8));
		buf[(64-y)*16+(x/8)]|=a;
	}

}

//******************************************************
//*************函数部分********************************/
/* 名字：mirror_point_12864()                         */
/* 功能：图象多点镜像描点函数                         */
/* 局部变量：取值范围 :y:[1,128]                      */
/*           像距为：h:[1,63]  中心线：center:[1,64]  */
/* 全局变量：无                                       */
/* 返回值：  无                                       */
/* 注意！！：两点之间的距离不得超过64（12864只有64行）*/
//*****************************************************/
void mirror_point_12864(uchar center,uchar x,uchar h,uchar *buf)
{
	uchar a=0,i,high=0;
	if (h>31)  high = 31;
	else       high = h;
	
	if (!(x%8))
	{
		a|=(1<<0);
		for (i=0;i<high;i++)
		{
			buf[(64-center+i)*16+(x/8)-1]|=a;
			buf[(64-center-i)*16+(x/8)-1]|=a;
		}
	}
	
	else
	{
		a|=(1<<(8-x%8));
		for (i=0;i<high;i++)
		{
			buf[(64-center+i)*16+(x/8)]|=a;
			buf[(64-center-i)*16+(x/8)]|=a;
		}
	}


}



//******************************************************
//*************函数部分********************************/
/* 名字：clear_print_12864()                          */
/* 功能：清除图象数据存储区                           */
/* 局部变量：取值范围 :y:[1,128]                      */
/*                                                    */
/* 全局变量：无                                       */
/* 返回值：  无                                       */
/* 注意                                               */
//*****************************************************/
void clear_print_12864(uchar *buffer)
{
	int i;
	for (i=0;i<1024;i++)
	{
		buffer[i] = 0x00;
	}
	drawpic_12864(buffer);
}



//******************************************************
//*************函数部分********************************/
/* 名字：subsection_point_12864()                     */
/* 功能：分段描点函数                                 */
/* 局部变量：                                         */
/*                                                    */
/* 全局变量：无                                       */
/* 返回值：  无                                       */
/* 注意！！：两点之间的距离不得超过64（12864只有64行）*/
//*****************************************************/
void  subsection_point_12864(uchar x,uchar center,uchar h,uchar *buf)
{
        uchar a=0,i,high=0;
	if (h>63)  high = 63;
	else       high = h;
	
	if (!(x%8))
	{
		a|=(1<<0);
		for (i=0;i<high;i++)
		{
			buf[(64-center-i)*16+(x/8)-1]|=a;
		}
	}
	
	else
	{
		a|=(1<<(8-x%8));
		for (i=0;i<high;i++)
		{
			buf[(64-center-i)*16+(x/8)]|=a;
		}
		
	}
	

}


//*************函数部分********************************/
/* 名字：line_piont()                                 */
/* 功能：画横线函数                                   */
/* 局部变量：                                         */
/*                                                    */
/* 全局变量：无                                       */
/* 返回值：  无                                       */
/* 注意！！：两点之间的距离不得超过128（12864只有128宽*/
//*****************************************************/
void line_piont(int x0,int y0,int x1,uchar *buf)
{
	uchar i;
	for (i=x0;i<x1;i++)
	{
		plot_12864(i,y0,buf);
	}
	
}

//*************函数部分********************************/
/* 名字：load_magic_12864()                          */
/* 功能：加载频谱魔幻花样                             */
/* 局部变量：取值范围 :y:[1,128]                      */
/*                                                    */
/* 全局变量：无                                       */
/* 返回值：  无                                       */
/* 注意                                               */
//*****************************************************/
void load_magic_12864(uchar *buffer)
{
	uchar i,h;
	
	for (i=0;i<16;i++)
	{
		h= buffer[i];
		line_piont(i*8+1,h+1,i*8+8,music);
		line_piont(i*8+1,h+2,i*8+8,music);
	}
}


/********************************************************
 * 名称：draw_Line()
    采用布兰森汉姆(Bresenham)算法画线
    * 功能：任意两点间的直线。根据硬件特点，实现加速。
    * 入口参数：x0     直线起点所在行的位置
    *       y0     直线起点所在列的位置
    *       x1     直线终点所在行的位置
‘       y1     直线终点所在列的位置
* 出口参数：   无
* 说明：操作失败原因是指定地址超出缓冲区范围。
*********************************************************/
void draw_line(int x0,int y0,int x1,int y1)
{
	int temp;
	int dx,dy;               //定义起点到终点的横、纵坐标增加值
	int s1,s2,status,i;
	int Dx,Dy,sub;

	dx=x1-x0;
	
	if(dx>=0)                 //X的方向是增加的
		s1=1;
	else                   //X的方向是降低的
		s1=-1;
	dy=y1-y0;                 //判断Y的方向是增加还是降到的
	if(dy>=0)   s2=1;

	else        s2=-1;

	Dx=abs_sub(x1,x0);           //计算横、纵标志增加值的绝对值
	Dy=abs_sub(y1,y0);
	if(Dy>Dx)               //
	{                     //以45度角为分界线，靠进Y轴是status=1,靠近X轴是status=0
		temp=Dx;
		Dx=Dy;
		Dy=temp;
		status=1;
	}
	else  status=0;


//********判断垂直线和水平线*******************************************
	//if(dx==0)                 //横向上没有增量，画一条水平线
	//line_piont(x0,y0,x1,music);
	//if(dy==0)                 //纵向上没有增量，画一条垂直线
	// subsection_point_12864(x0,y0,y1,music);


//*********Bresenham算法画任意两点间的直线*****************************
	sub=2*Dy-Dx;               //第1次判断下个点的位置
	for(i=0;i<Dx+1;i++)
	{
		plot_12864(x0,y0,music);           //画点
		if(sub>=0)
		{
			if(status==1)             //在靠近Y轴区，x值加1
				x0+=s1;
			else                   //在靠近X轴区，y值加1
				y0+=s2;
			sub-=2*Dx;               //判断下下个点的位置
		}
		if(status==1)
			y0+=s2;
		else
			x0+=s1;
		sub+=2*Dy;
	}


}

/***************************************************************
名称：Draw_circle (在任意位置画圆)
说明：使用Bresenham法画1/8个圆，在用对称性画出其他的7/8个圆

    按下图把圆分为8份
        0
      7   1
    6       2
      5   3
        4
*****************************************************************/
void draw_circle(uchar x0,uchar y0,uchar r)
{
	int a,b;
	int di;
	a=0;
	b=r;
	di=3-2*r;             //判断下个点位置的标志
	while(a<=b)
	{    //plot_12864(uchar x,uchar y,uchar *buf)
		plot_12864(x0-b,y0-a,music);           //3
		plot_12864(x0+b,y0-a,music);           //0
		plot_12864(x0-a,y0+b,music);           //1
		plot_12864(x0-b,y0-a,music);           //7
		plot_12864(x0-a,y0-b,music);           //2
		plot_12864(x0+b,y0+a,music);           //4
		plot_12864(x0+a,y0-b,music);           //5
		plot_12864(x0+a,y0+b,music);           //6
		plot_12864(x0-b,y0+a,music);
		a++;
		/***使用Bresenham算法画圆**/
		if(di<0)
			di +=4*a+6;
		else
		{
			di +=10+4*(a-b);
			b--;
		}
		plot_12864(x0+a,y0+b,music);

	}
}

void draw_rectangle(uchar x0,uchar y1)
{
	draw_line(x0*2+1,0,x0*2+1,y1);
	draw_line(x0*2+2,0,x0*2+2,y1);
}

void draw_rectangles(int arr[])
{
	int i;
	for(i=0;i<64;++i){
		draw_rectangle(i,arr[i]);
	}
}
void cfy_print_num(int32_t num)
{
	unsigned char arr[10];
	int i;
	for(i=0;num>0;++i){
		arr[i]=num%10;
		num/=10;
	}
	locatexy_12864(0,0);
	for(i-=1;i>=0;--i)
		writedata_12864(arr[i]+'0');
}


/* int main2(void) */
/* { */
/* 	int arr[64]; */
/* 	int i; */
/* 	for(i=0;i<64;++i){ */
/* 		arr[i]=i; */
/* 	} */
/* 	arr[63]=64; */
/* 	Init_12864(); */
/* 	drawpic_12864(music); */
/* 	draw_rectangles(arr); */
/* 	drawpic_12864(music); */
/* 	while(1); */
/* } */

