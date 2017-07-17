#include "STC15F2K60S2.H"
#include "key.h"
#include "display.h"
#include "ClockSystem.h"


void Delay50ms()		//@12.000MHz
{
	unsigned char i, j, k;

	i = 3;
	j = 72;
	k = 161;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
    system_init();
    while(1)
    {
        Delay50ms();		//@12.000MHz
		Delay50ms();		//@12.000MHz
		Delay50ms();		//@12.000MHz
		//Delay50ms();		//@12.000MHz
        system_update();
        //DisplayNums(Get_Key());
    }
}
