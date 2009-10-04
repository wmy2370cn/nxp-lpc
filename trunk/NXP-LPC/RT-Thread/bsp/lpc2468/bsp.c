/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/
 
#include <rtthread.h>
#include <LPC24xx.H> 
     

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

void LED_On(rt_uint8_t led_num)
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

void LED_Off(rt_uint8_t led_num)
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

void SetLed( rt_uint8_t uLedID,rt_uint8_t bState)
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
