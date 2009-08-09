

#include <rtthread.h>
#include "applib.h"
#include "lpc23xx.h"
#include "smartarm2300.h"

static void LED_HC595SendByte(INT8U dat)
{
	INT32U i = 0;

	IOCLR1 = GPIO1_SPInCS;

	for(i=0; i<8; i++)
	{
		IOCLR1 = GPIO1_SPICLK;
		if((dat & 0x80) == 0)
		{
			IOSET1 = GPIO1_SPIMOSI;
		}
		else
		{
			IOCLR1 = GPIO1_SPIMOSI;
		}
		dat = dat << 1;
		IOSET1 = GPIO1_SPICLK; 
	}

	IOSET1 = GPIO1_SPInCS;	 
}

/*
*********************************************************************************************************
*                                             LED ON
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to be ON
*                      1    turns ON LED1  on the board
*                      .
*                      .
*                      8    turns ON LED8 on the board
*
* Returns     : None
*********************************************************************************************************
*/
static rt_uint8_t LEDS_stat = (0);	
void  LED_On (rt_uint8_t led)
{
	if(led == 0)
	{
		LEDS_stat = ~0;
	}
	else if(led < 9)
	{
		led--;
		LEDS_stat = LEDS_stat | (0x01 <<led);
	}

	LED_HC595SendByte(LEDS_stat);
}

/*
*********************************************************************************************************
*                                             LED OFF
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want ALL the LEDs to be OFF
*                      1    turns OFF LED1  on the board
*                      .
*                      .
*                      8    turns OFF LED8 on the board
*
* Returns     : None
*********************************************************************************************************
*/

void  LED_Off (rt_uint8_t led)
{
	if(led == 0)
	{
		LEDS_stat = 0;
	}
	else if(led < 9)
	{
		led--;
		LEDS_stat = LEDS_stat & ~(0x01 <<led);
	}

	LED_HC595SendByte(LEDS_stat);

}

 
void SetLed( INT8U uLedID,INT8U bState)
{
	if (bState)
	{
		LED_On(uLedID);
	}
	else
	{
		LED_Off(uLedID);
	}
}

void FlickerLed( )
{
	INT32U i = 0;
	for (i = 1; i < 9; i++)
	{
		SetLed(i,TRUE);	 
	}
	rt_thread_delay(100);
	for (i = 1; i < 9; i++)
	{
		SetLed(i,FALSE);	 
	}

	rt_thread_delay(100);
	for (i = 1; i < 9; i++)
	{
		SetLed(i,TRUE);	 
	}
	rt_thread_delay(100);
	for (i = 1; i < 9; i++)
	{
		SetLed(i,FALSE);	 
	}
	rt_thread_delay(100);
	for (i = 1; i < 9; i++)
	{
		SetLed(i,TRUE);	 
	}
	for (i = 1; i < 9; i++)
	{
		SetLed(i,TRUE);
		rt_thread_delay(100);
		SetLed(i,FALSE);
	}
	rt_thread_delay(100);

	for (i = 1; i < 9; i++)
	{
		SetLed(i,FALSE);	 
	}
	for (i = 8; ;i--)
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
 	int *ptr = RT_NULL;
	 int count = 0;
	 FlickerLed( );

	 	ptr = (int *) rt_malloc (64);
	if (ptr == RT_NULL)
	{
		rt_kprintf("malloc ptr failed\n");
 
	}
	else
	rt_memset(	ptr, 0, 64);


	 while (1)
	 {
		 rt_kprintf("Thread2 count:%d\n", count++);
		 rt_thread_delay(50);   
		 SetLed(1,TRUE);
		 rt_thread_delay(50);
		 SetLed(1,FALSE);
	 }
 }
