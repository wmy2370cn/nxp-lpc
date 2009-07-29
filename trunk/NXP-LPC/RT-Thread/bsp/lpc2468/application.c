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


typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

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


char thread1_stack[512];
char thread2_stack[512];

struct rt_thread thread1;
struct rt_thread thread2;

void thread1_entry(void* parameter)
{
	int i;

	while (1)
	{
		for (i = 0; i < 10; i ++)
		{
			rt_kprintf("%d\n", i);
			rt_thread_delay(100);
		}
	}
}

void thread2_entry(void* parameter)
{
	int count = 0;
	while (1)
	{
		rt_kprintf("Thread2 count:%d\n", count++);
		rt_thread_delay(50);   
		SetLed(0,1);
		rt_thread_delay(50);
		SetLed(0,0);
	}
}

int rt_application_init()
{
 
	rt_thread_init(&thread1,
		"thread1",
		thread1_entry, RT_NULL,
		&thread1_stack[0], sizeof(thread1_stack),
		20, 10);

	rt_thread_init(&thread2,
		"thread2",
		thread2_entry, RT_NULL,
		&thread2_stack[0], sizeof(thread2_stack),
		25, 8);

	rt_thread_startup(&thread1);
	rt_thread_startup(&thread2);

	return 0;
}
  

/*@}*/
