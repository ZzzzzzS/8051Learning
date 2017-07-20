/**********************718����ʵ���ҿ���������*********************
*  ��д��718����ʵ����	����ZZS
*  ƽ̨��stc15K60s2
*  ˵�����������ߵ�ˮƽ���ޣ����в���֮�����������½⡣
*		 �����Ҷ࿴�������ֲᡣ
*		 �����⣬����ϵ��QQ523939208     by 718 ZZS       
******************************************************************/

#include "ClockSystem.h"
#include "STC15F2K60S2.H"
#include "data.h"
#include "key.h"
#include "display.h"

/****��������****/
void Timer_init();
void data_init();
void Clock_interface();
void Timer_interface();
void StopWatch_Interface();
void Timeset(time *Base,char AddMS);
void T0_isr();     //interrupt 1 


/****��������****/
void Timer_init()
{
    AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x28;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
    ET0=1;			//ʹ�ܶ�ʱ��0�ж�

    IT0=1;		   //�趨Ϊ�½��ش���     IT0=0;�½��غ������ؾ��ɴ���
	EX0=1;		   //�����ⲿ�ж�0
	EA=1;		   //�������ж�
}


void data_init()
{

    ClockBase.CurrentMode=StopWatch;        //����Ĭ�Ͻ���Ϊʱ�ӽ���

    /****����ʱ��****/
    ClockBase.Clock.CurrentInterface=Start;  //���õ�ǰģʽ�ͳ�ʼ��ʱ��
    ClockBase.Clock.CurrentTime.MS=0;
    ClockBase.Clock.CurrentTime.S=0;
    ClockBase.Clock.CurrentTime.M=0;
    ClockBase.Clock.CurrentTime.H=0;

    /****�������****/
    ClockBase.StopWatch.CurrentInterface=Setting;   //���õ�ǰģʽ�ͳ�ʼ��ʱ��
    ClockBase.StopWatch.CurrentTime.MS=0;
    ClockBase.StopWatch.CurrentTime.S=0;
    ClockBase.StopWatch.CurrentTime.M=0;
    ClockBase.StopWatch.CurrentTime.H=0;

    /****���ö�ʱ��****/
    ClockBase.Timer.CurrentInterface=Setting;   //���õ�ǰģʽ�ͳ�ʼ��ʱ��
    ClockBase.Timer.CurrentTime.MS=0;
    ClockBase.Timer.CurrentTime.S=0;
    ClockBase.Timer.CurrentTime.M=0;
    ClockBase.Timer.CurrentTime.H=0;
}

void system_init()
{
    Timer_init();               //���ö�ʱ����ʼ��
    data_init();                //�����ʼ��
}


void Timeset(time *Base,char AddMS)
{
    if(AddMS==0||AddMS>=100||AddMS<=-100)
        return;

    if(AddMS>0)
    {
        Base->MS+=AddMS;
        Base->S+=(Base->MS/1000);
        Base->M+=(Base->S/60);
        Base->H+=(Base->M/60);

        Base->MS=(Base->MS%1000);
        Base->S=(Base->S%60);
        Base->M=(Base->M%60);
        Base->H=(Base->H%24);
    }
    else
    {
        AddMS=-AddMS;

        Base->MS-=AddMS;
        if(Base->MS<0)
        {
            Base->MS=999;
            Base->S--;
        }
        if(Base->S<0)
        {
            Base->M--;
            Base->S=59;
        }
        if(Base->M<0)
        {
            Base->H--;
            Base->M=59;
        }
        if(Base->H<0)
        {
            Base->H=23;
        }
    }
}

void Timer_interface()
{
    switch(ClockBase.Timer.CurrentInterface)
    {
        case Setting:
            if(Get_Key()==SecondAdd_Button)
            {
                ClockBase.Timer.CurrentTime.S+=1;
                if(ClockBase.Timer.CurrentTime.S>60)
                {
                    ClockBase.Timer.CurrentTime.M++;
                    ClockBase.Timer.CurrentTime.S=0;
                }
            }
            else if(Get_Key()==SecondCut_Button)
            {
                ClockBase.Timer.CurrentTime.S-=1;
                if(ClockBase.Timer.CurrentTime.S<0)
                {
                    ClockBase.Timer.CurrentTime.M--;
                    ClockBase.Timer.CurrentTime.S=59;
                }
            }
            else if(Get_Key()==MinAdd_Button)
            {
                ClockBase.Timer.CurrentTime.M++;
            }
            else if(Get_Key()==MinCut_Button)
            {
                ClockBase.Timer.CurrentTime.M--;
                if(ClockBase.Timer.CurrentTime.M<=0)
                    ClockBase.Timer.CurrentTime.M=0;
            }

            if(Get_Key()==InterfaceShifting_Button)
            {
                ClockBase.Timer.CurrentInterface=Start;
            }
            break;
        
        case Start:
            if(Get_Key()==InterfaceShifting_Button)
            {
                ClockBase.Timer.CurrentInterface=Setting;
            }
            break;
        default:
            ClockBase.Timer.CurrentInterface=Start;
        
    }
}

void Clock_interface()
{
    switch(ClockBase.Clock.CurrentInterface)
    {
        case Start:
            if(Get_Key()==SecondAdd_Button)
            {

            }
            else if(Get_Key()==SecondCut_Button)
            {

            }
            else if(Get_Key()==MinAdd_Button)
            {
                ClockBase.Clock.CurrentTime.M++;
            }
            else if(Get_Key()==MinCut_Button)
            {
                ClockBase.Clock.CurrentTime.M--;
                if(ClockBase.Clock.CurrentTime.M<=0)
                    ClockBase.Clock.CurrentTime.M=59;
            }
            else if(Get_Key()==HoursAdd_Button)
            {
                ClockBase.Clock.CurrentTime.H++;
            }
            else if(Get_Key()==HoursCut_Button)
            {
                ClockBase.Clock.CurrentTime.H--;
                if(ClockBase.Clock.CurrentTime.H<=0)
                    ClockBase.Clock.CurrentTime.H=23;
            }
            break;
        
        default:
            ClockBase.Clock.CurrentInterface=Start;
    }
}

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void StopWatch_Interface()
{
    switch(ClockBase.StopWatch.CurrentInterface)
    {
        case Start:
            if(Get_Key()==InterfaceShifting_Button)
            {
                ClockBase.StopWatch.CurrentInterface=Setting;
            }
            /*if(P34==0&&ClockBase.StopWatch.CurrentTime.S>3)
            {
                ClockBase.StopWatch.CurrentInterface=Setting;
            }*/
            break;
        
        case Setting:
            if(Get_Key()==InterfaceShifting_Button)
            {
                ClockBase.StopWatch.CurrentInterface=Start;
                ClockBase.StopWatch.CurrentTime.MS=0;
                ClockBase.StopWatch.CurrentTime.S=0;
                ClockBase.StopWatch.CurrentTime.M=0;
                ClockBase.StopWatch.CurrentTime.H=0;
            }
            /*if(P34==0)
            {
                ClockBase.StopWatch.CurrentInterface=Start;
				ClockBase.StopWatch.CurrentTime.MS=0;
                ClockBase.StopWatch.CurrentTime.S=0;
                ClockBase.StopWatch.CurrentTime.M=0;
                ClockBase.StopWatch.CurrentTime.H=0;
                while(P34==0);
				Delay1000ms();
            }*/
            break;
        
        default:
            ClockBase.StopWatch.CurrentInterface=Setting;
    }
}

void system_update()
{
    switch(ClockBase.CurrentMode)       //ѡ����ʾ��ͬ�Ľ���
    {
        case Timer:
            P00=0;
            P01=1;
            P02=1;
            Timer_interface();
            //DisplayTime(ClockBase.Timer.CurrentTime.H,ClockBase.Timer.CurrentTime.M,ClockBase.Timer.CurrentTime.S);
            break;
        
        case StopWatch:
            P00=1;
            P01=0;
            P02=1;
            StopWatch_Interface();
            //DisplayStopWatch(ClockBase.StopWatch.CurrentTime.M,ClockBase.StopWatch.CurrentTime.S,ClockBase.StopWatch.CurrentTime.MS);
            break;
        
        case Clock:
            P00=1;
            P01=1;
            P02=0;
            Clock_interface();
            //DisplayTime(ClockBase.Clock.CurrentTime.H,ClockBase.Clock.CurrentTime.M,ClockBase.Clock.CurrentTime.S);
            break;
        
        default:
            ClockBase.CurrentMode=Clock;    //��������Ǿ͵�����clock����
    }

    if(Get_Key()==ModeShifting_Button)
    {
        ClockBase.CurrentMode++;
        if(ClockBase.CurrentMode==MAX_Mode)
            ClockBase.CurrentMode=Timer;
    }
}

void T0_isr() interrupt 1 
{
    if(ClockBase.StopWatch.CurrentInterface==Start)
    {
        Timeset(&(ClockBase.StopWatch.CurrentTime),5);
    }
    if(ClockBase.Clock.CurrentInterface==Start)
    {
        Timeset(&(ClockBase.Clock.CurrentTime),5);
    }
    if(ClockBase.Timer.CurrentInterface==Start)
    {
        Timeset(&(ClockBase.Timer.CurrentTime),-5);
        if(ClockBase.Timer.CurrentTime.H==23&&ClockBase.Timer.CurrentTime.M==59&&ClockBase.Timer.CurrentTime.S==59)
        {
            ClockBase.Timer.CurrentInterface=Setting;
            ClockBase.CurrentMode=Timer;
            P07=0;
            ClockBase.Timer.CurrentTime.H=0;
            ClockBase.Timer.CurrentTime.M=0;
            ClockBase.Timer.CurrentTime.S=0;
            ClockBase.Timer.CurrentTime.MS=0;
        }
        else
            P07=1;
    } 

	switch(ClockBase.CurrentMode)       //ѡ����ʾ��ͬ�Ľ���
    {
        case Timer:
            DisplayTime(ClockBase.Timer.CurrentTime.H,ClockBase.Timer.CurrentTime.M,ClockBase.Timer.CurrentTime.S);
            break;
        
        case StopWatch:
            DisplayStopWatch(ClockBase.StopWatch.CurrentTime.M,ClockBase.StopWatch.CurrentTime.S,ClockBase.StopWatch.CurrentTime.MS);
            break;
        case Clock:
            DisplayTime(ClockBase.Clock.CurrentTime.H,ClockBase.Clock.CurrentTime.M,ClockBase.Clock.CurrentTime.S);
            break;
    }
}


void exint0_isr() interrupt 0
{
     switch(ClockBase.StopWatch.CurrentInterface)
    {
        case Start:
            if(ClockBase.StopWatch.CurrentTime.S>3)
            {
                ClockBase.StopWatch.CurrentInterface=Setting;
				Delay1000ms();
            }
            break;
        
        case Setting:
            {
                ClockBase.StopWatch.CurrentInterface=Start;
				ClockBase.StopWatch.CurrentTime.MS=0;
                ClockBase.StopWatch.CurrentTime.S=0;
                ClockBase.StopWatch.CurrentTime.M=0;
                ClockBase.StopWatch.CurrentTime.H=0;
            }
            break;
    }
}