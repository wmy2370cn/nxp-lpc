

#include <rtthread.h>
#include "applib.h"  
#include "bsp.h"
#include "nic.h"
 

void FlickerLed( )
{
	rt_uint32_t i = 0;
	for (i = 0; i < 11; i++)
	{
		SetLed(i,TRUE);	 
	}
	rt_thread_delay(100);
	for (i = 0; i < 11; i++)
	{
		SetLed(i,FALSE);	 
	}

	rt_thread_delay(100);
	for (i = 0; i < 11; i++)
	{
		SetLed(i,TRUE);	 
	}
	rt_thread_delay(100);
	for (i = 0; i < 11; i++)
	{
		SetLed(i,FALSE);	 
	}
	rt_thread_delay(100);
	for (i = 0; i < 11; i++)
	{
		SetLed(i,TRUE);	 
	}
	for (i = 0; i < 11; i++)
	{
		SetLed(i,TRUE);
		rt_thread_delay(100);
		SetLed(i,FALSE);
	}
	rt_thread_delay(100);

	for (i = 0; i < 11; i++)
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
	 int *ptr = RT_NULL;
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
