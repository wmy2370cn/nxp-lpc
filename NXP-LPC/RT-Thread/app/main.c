

#include <rtthread.h>
#include "applib.h"
#include "lpc2400.h"

//定义LED的IO控制脚
#define	LED1		1 << 19		//P2.19  
#define	LED2		1 << 30		//P0.30
#define	LED3		1 << 18		//P2.18
#define	LED4		1 << 24		//P3.24
#define	LED5		1 << 25		//P3.25
#define	LED6		1 << 29		//P0.29
#define	LED7		1 << 26		//P3.26
#define	LED8		1 << 25		//P2.25
#define	LED9		1 << 24 	//P2.24
#define	LED10		1 << 12		//P0.12
#define	LED11		0x80000000	//P0.31
#define	LED12		1 << 28		//P2.28

void hd_SetOneLED(INT8U led_num)
{	
	switch(led_num)
	{
	case 0:
		FIO2CLR =LED1;	break;
	case 1:
		FIO0CLR =LED2;	break;			
	case 2:
		FIO2CLR =LED3;	break;	
	case 3:
		FIO3CLR =LED4;	break;
	case 4:
		FIO3CLR =LED5;	break;
	case 5:
		FIO0CLR =LED6;	break;			
	case 6:
		FIO3CLR =LED7;	break;	
	case 7:
		FIO2CLR =LED8;	break;
	case 8:
		FIO2CLR =LED9;	break;
	case 9:
		FIO0CLR =LED10;	break;			
	case 10:
		FIO0CLR =LED11;	break;	
	case 11:
		FIO2CLR =LED12;	break;
	}
}

void hd_ClearOneLED(INT8U led_num)
{	
	switch(led_num)
	{
	case 0:
		FIO2SET =LED1;	break;
	case 1:
		FIO0SET =LED2;	break;			
	case 2:
		FIO2SET =LED3;	break;	
	case 3:
		FIO3SET =LED4;	break;
	case 4:
		FIO3SET =LED5;	break;
	case 5:
		FIO0SET =LED6;	break;			
	case 6:
		FIO3SET =LED7;	break;	
	case 7:
		FIO2SET =LED8;	break;
	case 8:
		FIO2SET =LED9;	break;
	case 9:
		FIO0SET =LED10;	break;			
	case 10:
		FIO0SET =LED11;	break;	
	case 11:
		FIO2SET =LED12;	break;
	}	
}
void SetLed( INT8U uLedID,INT8U bState)
{
	if (bState)
	{
		hd_SetOneLED(uLedID);
	}
	else
	{
		hd_ClearOneLED(uLedID);
	}
}

void FlickerLed( )
{
	INT32U i = 0;
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);	 
	}
	rt_thread_delay(100);
	for (i = 0; i < 12; i++)
	{
		SetLed(i,FALSE);	 
	}

	rt_thread_delay(100);
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);	 
	}
	rt_thread_delay(100);
	for (i = 0; i < 12; i++)
	{
		SetLed(i,FALSE);	 
	}
	rt_thread_delay(100);
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);	 
	}
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);
		rt_thread_delay(100);
		SetLed(i,FALSE);
	}
	rt_thread_delay(100);

	for (i = 0; i < 12; i++)
	{
		SetLed(i,FALSE);	 
	}
	for (i = 11; ;i--)
	{
		SetLed(i,TRUE);
		rt_thread_delay(100);
		SetLed(i,FALSE);
		if (i == 0)
			break;
	}
} 
 
 void app_thread_entry(void* parameter)
 {
	 int count = 0;
	 FlickerLed( );

	 while (1)
	 {
		 rt_kprintf("Thread2 count:%d\n", count++);
		 rt_thread_delay(50);   
		 SetLed(0,TRUE);
		 rt_thread_delay(50);
		 SetLed(0,FALSE);
	 }
 }
