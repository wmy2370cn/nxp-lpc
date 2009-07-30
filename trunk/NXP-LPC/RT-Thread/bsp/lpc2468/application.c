/*
 * File      : app.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-12-11     xuxinming    the first version
 */

#include <rtthread.h>

/**
 * @addtogroup LPC2478
 */
/*@{*/

char app_thread_stack[512];
struct rt_thread app_thread;

int rt_application_init()
{ 
	extern void app_thread_entry(void* parameter);
	rt_thread_init(&app_thread,"app_thread",app_thread_entry, RT_NULL,	&app_thread_stack[0], sizeof(app_thread_stack),
		20, 10);

	rt_thread_startup(&app_thread);
	 
	return 0;
}
  

/*@}*/
