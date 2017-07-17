#include<reg52.h>
int j=0;
char code sz[]={"hello "};
char  sz2[3];
void inie()
{
	EA=1;
	ES=1;
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	SM0=0;
	SM1=1;
	REN=1;
	}
void main()
{
	int i=0;
	inie();
	while(1)
	{	
	 if(j>2)
	 {
		ES=0;
		for(i=0;i<=5;++i)
		{
			SBUF=sz[i];
			while(TI==0);
			TI=0;
		}
		for(i=0;i<=2;++i)
		{
			SBUF=sz2[i];
			while(TI==0);
			TI=0;
		}

		ES=1;
		j=0;;
	 }
	}
}
void ShuRu() interrupt 4
{
		RI=0;
		sz2[j]=SBUF;
		j++;
	}
	










	