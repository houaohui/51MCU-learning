#include <stc12c5a60s2.h>
#include <intrins.h>

sfr PLASF = 0x9d;

#define ADC_POWER 0x80           //ADC开关控制位
#define ADC_FLAG 0x10            //ADC转换完成标志位
#define ADC_START 0x08           //ADC开始转换位
#define ADC_SPEEDLL 0x00         //540 clocks
unsigned char dat,a,b,c;
unsigned char code ledchar[]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
/*-------------
延时函数
--------------*/
void delay(unsigned int n)
{
	unsigned int x;
	while(n--)
	{
		x=5000;
		while(x--);
	}
}
/*----------------
ADC初始化
----------------*/
void ADC_init()
{
	PLASF = 0x01;           //设置P10为ADC转换通道
	ADC_RES = 0;            //清除上次转化结果
	ADC_CONTR = ADC_POWER|ADC_SPEEDLL|ADC_START;
	delay(2);               //延时四个时钟周期
}

void main()
{
	P11=0;                  //使能P0
	ADC_init();
	IE=0xa0;                //打开ADC中断
	while(1);
}

void adc() interrupt 5 using 1
{
	ADC_CONTR&=!ADC_FLAG;     //清除转换标志位
	dat = ADC_RES;
	
	a=ledchar[dat%10];
	b=ledchar[dat/10%10];
	c=ledchar[dat/100%10];
	
	P20=1;P21=0;P22=1;P0=a;delay(1);P0=0xff;
	P20=0;P21=0;P22=1;P0=b;delay(1);P0=0xff;
	P20=1;P21=1;P22=0;P0=c;delay(1);P0=0xff;
	
	ADC_CONTR = ADC_POWER|ADC_SPEEDLL|ADC_START;
}





