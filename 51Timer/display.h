 #ifndef __DISPLAY_H__
 #define __DISPLAY_H__
 #include "STC15F2K60S2.h"

/*************	IO¿Ú¶¨Òå	**************/
sbit    P_HC595_OE    = P3^7;	//pin 13    OE      out enabled
sbit	P_HC595_SER   = P4^5;	//pin 14	SER		data input
sbit	P_HC595_RCLK  = P4^4;	//pin 12	RCLk	store (latch) clock
sbit	P_HC595_SRCLK = P4^2;	//pin 11	SRCLK	Shift data clock

extern void DisplayNums(unsigned  long int nums);
extern void DisplayTime(char HH,char MM,char SS);
extern void DisplayNum(unsigned char num,unsigned char place);
#endif //__DISPLAY_H__