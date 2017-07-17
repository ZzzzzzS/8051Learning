/**********************718����ʵ���ҿ���������*********************
*  ��д��718����ʵ����	����jcp
*  ƽ̨��stc15K60s2
*  ˵�����������ߵ�ˮƽ���ޣ����в���֮�����������½⡣
*		 �����Ҷ࿴�������ֲᡣ
*		 �����⣬����ϵ��QQ517776106     by 718 jcp       
******************************************************************/

#include "display.h"

/*************************����˵��******************************
������8λ����ܶ�̬��ʾ����
****************************************************************/



unsigned char code tableduan[]={
// 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71   // ����
};
unsigned char code tablewei[]={
	//1    2    3    4    5    6   7    8  
	0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f                                           //λ��
};
/**************** ��HC595����һ���ֽں��� ******************/
void Send_595(unsigned char dat)
{		
	unsigned char	i;
	P_HC595_OE = 0;           //hc595���ʹ��
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
/************************* ��ʾ���� ***************************/
/*
��������  void DisplayNum()
���ã�    ����ܵ�λ��ʾ
������	  num   ������ʾ������ 0~9,  
		  place ������ʾ��λ��  0��Ӧ���λ
���أ�    void
˵����    ��Ҫ���595����ʹ��

*/
void DisplayNum(unsigned char num,unsigned char place)
{	
	Send_595(tablewei[place]);	  	    //���λ�� 
	Send_595(tableduan[num]);	        //�������

	P_HC595_RCLK = 1;
	P_HC595_RCLK = 0;							//�����������
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
/************************ m^n���� *****************************/
/*
��������  unsigned  long int  mypow
���ã�    ��m��n�η�
������	  m   n  
���أ�    m��n�η�
˵����    
*/
unsigned  long int  mypow(unsigned char m,unsigned char n)
{
	unsigned long int result=1;	 
	while(n--)result*=m;    
	return result;
}
/***************************��ʱ����****************************/

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
/************************* ��ʾ���� ***************************/
/*
��������  void DisplayNums()
���ã�    ����ܶ�λ��ʾ
������	  nums   ������ʾ������  
���أ�    void
˵����    ��Ҫ���595�������ʱ������mypow����ʹ��
*/
	
void DisplayNums(unsigned  long int nums)
{
	unsigned char t=0;
	unsigned char temp;
	for(t=0;t<8;t++)
	{		
		Send_595(tablewei[t]);	  	        //���λ�� 
		temp=(nums/mypow(10,8-t-1))%10;	    //�����λҪ��ʾ����
		Send_595(tableduan[temp]);	        //�������

		P_HC595_RCLK = 1;
		P_HC595_RCLK = 0;							//�����������

		Delay1ms();	
	}
}