/**********************718创新实验室开发板例程*********************
*  编写：718创新实验室	——ZZS
*  平台：stc15K60s2
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。
*		 有问题，请联系我QQ523939208     by 718 ZZS       
******************************************************************/

#include "ClockSystem.h"
#include "STC15F2K60S2.H"
#include "data.h"
#include "key.h"
#include "display.h"

/****声明函数****/
void Timer_init();
void data_init();
void Clock_interface();
void Timer_interface();
void StopWatch_Interface();
void Timeset(time *Base,char AddMS);
void T0_isr();     //interrupt 1 


/****函数定义****/
void Timer_init()
{
    AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x28;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0=1;			//使能定时器0中断

    IT0=1;		   //设定为下降沿触发     IT0=0;下降沿和上升沿均可触发
	EX0=1;		   //开启外部中断0
	EA=1;		   //开启总中断
}


void data_init()
{

    ClockBase.CurrentMode=StopWatch;        //设置默认界面为时钟界面

    /****设置时钟****/
    ClockBase.Clock.CurrentInterface=Start;  //设置当前模式和初始化时间
    ClockBase.Clock.CurrentTime.MS=0;
    ClockBase.Clock.CurrentTime.S=0;
    ClockBase.Clock.CurrentTime.M=0;
    ClockBase.Clock.CurrentTime.H=0;

    /****设置秒表****/
    ClockBase.StopWatch.CurrentInterface=Setting;   //设置当前模式和初始化时间
    ClockBase.StopWatch.CurrentTime.MS=0;
    ClockBase.StopWatch.CurrentTime.S=0;
    ClockBase.StopWatch.CurrentTime.M=0;
    ClockBase.StopWatch.CurrentTime.H=0;

    /****设置定时器****/
    ClockBase.Timer.CurrentInterface=Setting;   //设置当前模式和初始化时间
    ClockBase.Timer.CurrentTime.MS=0;
    ClockBase.Timer.CurrentTime.S=0;
    ClockBase.Timer.CurrentTime.M=0;
    ClockBase.Timer.CurrentTime.H=0;
}

void system_init()
{
    Timer_init();               //设置定时器初始化
    data_init();                //界面初始化
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
    switch(ClockBase.CurrentMode)       //选择显示不同的界面
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
            ClockBase.CurrentMode=Clock;    //如果都不是就调整成clock界面
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

	switch(ClockBase.CurrentMode)       //选择显示不同的界面
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