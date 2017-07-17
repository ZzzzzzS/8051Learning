#include "key.h"

void Delay_Time(char k)		//@11.0592MHz
{
	unsigned char i;
	i = 25;
	while (--i);
}

char Get_Key()
  {
      char key=0;
   unsigned temp;
    P2=0xfe;
    temp=P2;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      Delay_Time(10);
      if(temp!=0xf0)
      {
        temp=P2;
        switch(temp)
        {
          case 0xee:
               key=0;
               break;

          case 0xde:
               key=1;
               break;

          case 0xbe:
               key=2;
               break;

          case 0x7e:
               key=3;
               break;
         }
      }
    }





    P2=0xfd;
    temp=P2;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      Delay_Time(10);
      if(temp!=0xf0)
      {
        temp=P2;
        switch(temp)
        {
          case 0xed:
               key=4;
               break;

          case 0xdd:
               key=5;
               break;

          case 0xbd:
               key=6;
               break;

          case 0x7d:
               key=7;
               break;
         }
      }
   }






    P2=0xfb;
    temp=P2;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      Delay_Time(10);
      if(temp!=0xf0)
      {
        temp=P2;
        switch(temp)
        {
          case 0xeb:
               key=8;
               break;

          case 0xdb:
               key=9;
               break;

          case 0xbb:
               key=10;
               break;

          case 0x7b:
               key=11;
               break;
         }
      }
   }








    P2=0xf7;
    temp=P2;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      Delay_Time(10);
      if(temp!=0xf0)
      {
        temp=P2;
        switch(temp)
        {
          case 0xe7:
               key=12;
               break;

          case 0xd7:
               key=13;
               break;

          case 0xb7:
               key=14;
               break;

          case 0x77:
               key=15;
               break;
         }
      }
    }
    return key;
  }