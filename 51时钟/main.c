#include<reg52.h>
 char code Table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80,0x0};
		     		//0   1    2    3     4   5     6    7    8   9     .	空
sbit WeiMa=P2^7;
sbit DuanMa=P2^6;

sbit Key1=P3^4;
sbit Key2=P3^5;
sbit Key3=P3^6;
sbit Key4=P3^7;

int g_h=0,g_m=0,g_s=0;
int g_Tm=0,g_Ts=20,g_Th;
int g_KaiGuan=0;                                                  //功能启动开关

void Get_Key()													//键盘检测
{
	
	   if(Key1==0&&Key2!=0&&Key3!=0&&Key4!=0)
	  	{
	   		if(g_KaiGuan==0)
				g_h++;
			else if(g_KaiGuan==1)
				g_Tm++;
		}

	else if(Key1!=0&&Key2==0&&Key3!=0&&Key4!=0)
		{
			if(g_KaiGuan==0)
				g_h--;
			else if(g_KaiGuan==1)
				g_Tm--;
		}

	else if(Key1!=0&&Key2!=0&&Key3==0&&Key4!=0)
		{
			if(g_KaiGuan==0)
				g_m++;
			else if(g_KaiGuan==1)
				g_Ts++;
		}

	else if(Key1!=0&&Key2!=0&&Key3!=0&&Key4==0)
		{
			if(g_KaiGuan==0)
				g_m--;
			else if(g_KaiGuan==1)
				g_Ts--;
		}

	else if(Key1==0&&Key2==0&&Key3!=0&&Key4!=0)
		{
			if(g_KaiGuan==0)
				g_KaiGuan++;
			else if(g_KaiGuan==1)
				g_KaiGuan++;
			else if(g_KaiGuan==2||g_KaiGuan==3)
				g_KaiGuan=0;
		}
	else if(Key1!=0&&Key2!=0&&Key3==0&&Key4==0)
		{
			if(g_KaiGuan==2)
				g_KaiGuan=3;
			else if(g_KaiGuan==3)
				g_KaiGuan=2;
		}

}





void Delay_Time(int Time)										//延时
{
	int i,j;
	for(i=0;i<=Time;i++)
		for(j=0;j<=110;j++);
}





void Display(int h1, int h2,int m1,int m2 ,int s1,int s2) 		//公用输出
{
	WeiMa=1;
   P0=0xfe;
   WeiMa=0;
   DuanMa=1;
   P0=Table[h1];
   DuanMa=0;
   Delay_Time(1);

   WeiMa=1;
   P0=0xfd;
   WeiMa=0;
   DuanMa=1;
   P0=Table[h2];
   DuanMa=0;
   Delay_Time(1);

   WeiMa=1;
   P0=0xfb;
   WeiMa=0;
   DuanMa=1;
   P0=Table[m1];
   DuanMa=0;
    Delay_Time(1);

   WeiMa=1;
   P0=0xf7;
   WeiMa=0;
   DuanMa=1;
   P0=Table[m2];
   DuanMa=0;
   Delay_Time(1);

   WeiMa=1;
   P0=0xef;
   WeiMa=0;
   DuanMa=1;
   P0=Table[s1];
   DuanMa=0;
   Delay_Time(1);

   WeiMa=1;
   P0=0xdf;
   WeiMa=0;
   DuanMa=1;
   P0=Table[s2];
   DuanMa=0;
   Delay_Time(1);
 

   if(s2%2==0){
   WeiMa=1;
   P0=0xf5;
   WeiMa=0;
   DuanMa=1;
   P0=Table[10];
   DuanMa=0;
   Delay_Time(1);  
   }
}





void Function_Clock()
{
	int i;
	  g_s++;
	   if(g_s>=60)
	   {
	   		g_s=0;
			g_m++;
		}
		if(g_m==60)
		{
			g_m=0;
			g_h++;
		}
		if(g_h==24)
		{
			g_h=0;
		}
		if(g_s==-1)
		{
			g_s=59;
			g_m--;
		}
		if(g_m==-1)
		{
			g_m=59;
			g_h--;
		}
		if(g_h==-1)
		{
			g_h=23;
		}
		if(g_KaiGuan==0||(g_KaiGuan==3))
		 for(i=0;i<=75;i++)
			Display(g_h/10,g_h%10,g_m/10,g_m%10,g_s/10,g_s%10);
}



void Function_Timer()
{		  
	int i;
	if(g_KaiGuan==0);
	else if(g_KaiGuan==1)
	{
		if(g_Ts==60)
		{
			g_Ts=0;
			g_Tm++;
		}
		if(g_Tm==60)
		{
			g_Tm=0;
			g_Th++;
		}
		if(g_Th>99)
		{
			g_Th=99;
		}
		for(i=0;i<=75;i++)
			Display(g_Th/10,g_Th%10,g_Tm/10,g_Tm%10,g_Ts/10,g_Ts%10);	
	}
	else if(g_KaiGuan==2)
	{

		g_Ts--;
		if(g_Ts==-1)
		{
			g_Ts=59;
			g_Tm--;
		}
		if(g_Tm==-1)
		{
			g_Tm=59;
			g_Th--;
		}
		for(i=0;i<=75;i++)
		Display(g_Th/10,g_Th%10,g_Tm/10,g_Tm%10,g_Ts/10,g_Ts%10);
		if(g_Tm==0&&g_Ts==0&&g_Th==0)
		{
			for(i=0;i<=75;i++)
				Display(8,8,8,8,8,8);
			g_Ts++;
		}
	}
	else if(g_KaiGuan==3)
	{
		g_Ts--;
		if(g_Ts==-1)
		{
			g_Ts=59;
			g_Tm--;
		}
		if(g_Tm==-1)
		{
			g_Tm=59;
			g_Th--;
		}
		if(g_Tm==0&&g_Ts==0&&g_Th==0)
		{
			for(i=0;i<=75;i++)
			{
				Display(8,8,8,8,8,8);
			}
			g_s++;
			g_Ts++;
		}
	}	
}





void main()
{
	while(1)
	{
		Get_Key();
		Function_Clock();
		Function_Timer();
	}
	
 }





