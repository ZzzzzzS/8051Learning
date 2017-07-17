/**********************718创新实验室开发板例程*********************
*  编写：718创新实验室	——jcp
*  平台：stc15K60s2
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。
*		 有问题，请联系我QQ517776106     by 718 jcp       
******************************************************************/

#include "display.h"

/*************************功能说明******************************
程序中8位数码管动态显示数字
****************************************************************/



unsigned char code tableduan[]={
// 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71   // 段码
};
unsigned char code tablewei[]={
	//1    2    3    4    5    6   7    8  
	0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f                                           //位码
};
/**************** 向HC595发送一个字节函数 ******************/
void Send_595(unsigned char dat)
{		
	unsigned char	i;
	P_HC595_OE = 0;           //hc595输出使能
	for(i=0; i<8; i++)
	{
		if(dat & 0x80)
		{
			P_HC595_SER   = 1;
		}
		else
		{
			P_HC595_SER   = 0;
		}		
		P_HC595_SRCLK = 1;
		P_HC595_SRCLK = 0;
		dat <<= 1;
		
	}
}
/************************* 显示函数 ***************************/
/*
函数名：  void DisplayNum()
作用：    数码管单位显示
参数：	  num   ——显示的数字 0~9,  
		  place ——显示的位置  0对应最高位
返回：    void
说明：    需要配合595程序使用

*/
void DisplayNum(unsigned char num,unsigned char place)
{	
	Send_595(tablewei[place]);	  	    //输出位码 
	Send_595(tableduan[num]);	        //输出段码

	P_HC595_RCLK = 1;
	P_HC595_RCLK = 0;							//锁存输出数据
}

void DisplayTime(char HH,char MM,char SS)
{
	DisplayNum(HH/10,0);
	DisplayNum(HH%10,1);
	DisplayNum(MM/10,3);
	DisplayNum(MM%10,4);
	DisplayNum(SS/10,6);
	DisplayNum(SS%10,7);
}
/************************ m^n函数 *****************************/
/*
函数名：  unsigned  long int  mypow
作用：    求m的n次方
参数：	  m   n  
返回：    m的n次方
说明：    
*/
unsigned  long int  mypow(unsigned char m,unsigned char n)
{
	unsigned long int result=1;	 
	while(n--)result*=m;    
	return result;
}
/***************************延时函数****************************/

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}
/************************* 显示函数 ***************************/
/*
函数名：  void DisplayNums()
作用：    数码管多位显示
参数：	  nums   ——显示的数字  
返回：    void
说明：    需要配合595程序和延时函数和mypow函数使用
*/
	
void DisplayNums(unsigned  long int nums)
{
	unsigned char t=0;
	unsigned char temp;
	for(t=0;t<8;t++)
	{		
		Send_595(tablewei[t]);	  	        //输出位码 
		temp=(nums/mypow(10,8-t-1))%10;	    //求出次位要显示的数
		Send_595(tableduan[temp]);	        //输出段码

		P_HC595_RCLK = 1;
		P_HC595_RCLK = 0;							//锁存输出数据

		Delay1ms();	
	}
}