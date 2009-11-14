

#include <rtthread.h>
#include "applib.h"  
#include "bsp.h"
#include "nic.h"
#include "gui.h"

//TODO: 以后把这块统一处理
#include "memdata.h"
 

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


void time_cb_func(void *pdata)
{
	static INT8U nState = FALSE;

	SetLed(1,nState);
	nState = ! nState;
}
struct rt_timer tim2 ;
void main_app_thread_entry(void* parameter)
{															  
	int count = 0;
	
	
 	FlickerLed( );

 	InitMemData();

	InitGuiTask();

	rt_timer_init(&tim2,"soft_tim2",time_cb_func,RT_NULL,50,RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
	rt_timer_start(&tim2);

	while (1)
	{
		rt_kprintf("Thread2 count:%d\n", count++);
		rt_thread_delay(50);   
		SetLed(0,TRUE);
		rt_thread_delay(50);
		SetLed(0,FALSE);
	}
}
