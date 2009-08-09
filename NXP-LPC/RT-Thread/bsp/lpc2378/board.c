/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-12-11     xuxinming    first version
 */

#include <rtthread.h>
#include <rthw.h>

#include <LPC23xx.h>
#include "applib.h"
#include "board.h"
/* #define RT_BOARD_DEBUG */

#define DATA_COUNT 12000000/RT_TICK_PER_SECOND	/* T0MR0 = delayInMs * (Fpclk / 1000); */

/**
 * @addtogroup LPC2478
 */
/*@{*/

void rt_timer_handler(int vector)
{
#ifdef BOARD_DEBUG
	rt_kprintf("timer handler, increase a tick\n");
#endif

	T0IR |= 0x01;			/* clear interrupt flag */
	rt_tick_increase();
	VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/**
 * This function is used to display a string on console, normally, it's
 * invoked by rt_kprintf
 *
 * @param str the displayed string
 */
void rt_hw_console_output(const char* str)
{
	while (*str)
	{
		if (*str=='\n')
		{
			while (!(U0LSR & 0x20));
			U0THR = '\r';
		}
	
		while (!(U0LSR & 0x20));
		U0THR = *str;
		
		str ++;
	}
}



/*
*********************************************************************************************************
*                                          INITIALIZE I/Os
*
* Description : This function consolidates the I/O initialization for various modules.
*
* Arguements  : None
*
* Returns     : None
*
* Note(s)     : (1) Refer to the LPC2468 User Manual for a detailed pin assignment
*********************************************************************************************************
*/

static  void  GPIO_Init (void)
{
    rt_uint32_t  pinsel;


//    IO0DIR      =  0;
//    IO1DIR      =  0;
    IODIR0      =  0;	  // zhaoyk20081124
    IODIR1      =  0;	  // zhaoyk20081124
    FIO0DIR     =  0;
    FIO1DIR     =  0;
    FIO2DIR     =  0;
    FIO3DIR     =  0;
    FIO4DIR     =  0;

    FIO0MASK    =  0;
    FIO1MASK    =  0;
    FIO2MASK    =  0;
    FIO3MASK    =  0;
    FIO4MASK    =  0;

    PINSEL0     =  0;
    PINSEL1     =  0;
    PINSEL2     =  0;
    PINSEL3     =  0;
    PINSEL4     =  0;
    PINSEL5     =  0;
    PINSEL6     =  0;
    PINSEL7     =  0;
    PINSEL8     =  0;
    PINSEL9     =  0;
    PINSEL10    =  0;

#if (OS_VIEW_MODULE > 0) && (OS_VIEW_COMM_SEL == OS_VIEW_UART_0)
                                                                /* Configure P3.16 & P3.17 for UART1                        */
    pinsel          = PINSEL7;
    pinsel         &= 0xFFFFFFF0;
    pinsel         |= 0x0000000F;
    PINSEL7         = pinsel;
#endif

#if (OS_VIEW_MODULE > 0) && (OS_VIEW_COMM_SEL == OS_VIEW_UART_1)
                                                                /* Configure P3.16 & P3.17 for UART1                        */
    pinsel          = PINSEL7;
    pinsel         &= 0xFFFFFFF0;
    pinsel         |= 0x0000000F;
    PINSEL7         = pinsel;
#endif

                                                                /* Configure P0.28 & P0.27 for I2C0                         */
    pinsel          = PINSEL1;
    pinsel         &= 0xFC3FFFFF;
    pinsel         |= 0x01400000;
    PINSEL1         = pinsel;

                                                                /* Configure P2.10 for GPIO                                 */
    pinsel          = PINSEL4;
    pinsel         &= 0xFFCFFFFF;
    PINSEL4         = pinsel;
    FIO2DIR        &= ~DEF_BIT_10;


    FIO3DIR         = GPIO3_LCD_LIGHT;
    FIO3CLR         = GPIO3_LCD_LIGHT;
/*********************************************************************************************/

	PINSEL0 = PINSEL0 | 0x50;  //设置引脚P0.2,P0.3为通讯口UART0, for LPC22xx

                       /* Configure P1.20 output, P1.21 output, P1.23 input, P1.24 output, for GPIO output for LEDS         */
    IOSET1      =  GPIO1_SPInCS;	  // zhaoyk20081124
    IOCLR1      =  GPIO1_SPICLK;	  // zhaoyk20081124
    IODIR1      =  IODIR1 | GPIO1_SPICLK;	  // zhaoyk20081124
    IODIR1      =  IODIR1 | GPIO1_SPInCS;	  // zhaoyk20081124
    IODIR1      =  IODIR1 | GPIO1_SPIMOSI;	  // zhaoyk20081124

                                                                /* Configure P1.27 for GPIO output for Buzzer               */
    IODIR1      =  IODIR1 | GPIO1_BUZZER;	  // zhaoyk20081124
    IOSET1      =  GPIO1_BUZZER;	  // zhaoyk20081124

}

		

/**
 * This function will init LPC2478 board
 */
void rt_hw_board_init()
{
	extern void rt_serial_init(void);

	GPIO_Init();

	VICIntEnClr 	= 0xFFFFFFFFL;

	/* init hardware serial */
//	rt_serial_init();

	T0IR 	= 0xff; 
	T0TC 	= 0;
	T0MCR 	= 0x03; 
	T0MR0 	= (DATA_COUNT);

	rt_hw_interrupt_install(TIMER0_INT, rt_timer_handler, RT_NULL);	
	rt_hw_interrupt_umask(TIMER0_INT);

	T0TCR = 0x01; //enable timer0 counter


	
//	PCONP |= 3<<22;//使能定时器2，3
//	PCONP |= 1<<25;//使能Uart3
//	PCONP |= 1<<30;//使能Ethnet

 
}

#ifdef RT_USING_FINSH
//extern void finsh_notify(void);
void rt_serial_isr(int vector)
{
	rt_uint32_t iir;

	if (U0LSR & 0x01)
	{
		rt_uint8_t ch;
		
		while (U0LSR & 0x01)
		{
			ch = U0RBR;
			rt_serial_savechar(ch);
		}

		/* notify finsh shell thread */
	//	finsh_notify();
	}

	/* clear interrupt source */
	iir = U0IIR;

	/* acknowledge Interrupt */
	VICVectAddr = 0;
}

void rt_hw_finsh_init()
{
	/* init UART rx interrupt */
	U0IER = 0x01;

	/* install UART isr */
	rt_hw_interrupt_install(UART0_INT, rt_serial_isr, RT_NULL);
	rt_hw_interrupt_umask(UART0_INT);
}
#endif

/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a necessary 
**						place, change it as needed
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetInit(void)
{
	/* Add your codes here */
	return;
}

/******************************************************************************
** Function name:		GPIOResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**				function; User may change it as needed, but may not 
**				deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void GPIOResetInit( void )
{
	return;        
}

/******************************************************************************
** Function name:		ConfigurePLL
**
** Descriptions:		Configure PLL switching to main OSC instead of IRC
**						at power up and wake up from power down. 
**						This routine is used in TargetResetInit() and those
**						examples using power down and wake up such as
**						USB suspend to resume, ethernet WOL, and power management
**						example
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ConfigurePLL ( void )
{
  unsigned long MValue, NValue;

  if ( PLLSTAT & (1 << 25) )
  {
	PLLCON = 1;			/* Enable PLL, disconnected */
	PLLFEED = 0xaa;
	PLLFEED = 0x55;
  }

  PLLCON = 0;				/* Disable PLL, disconnected */
  PLLFEED = 0xaa;
  PLLFEED = 0x55;

  SCS |= 0x20;			/* Enable main OSC */
  while( !(SCS & 0x40) );	/* Wait until main OSC is usable */

  CLKSRCSEL = 0x1;		/* select main OSC, 12MHz, as the PLL clock source */

  PLLCFG = PLL_MValue | (PLL_NValue << 16);
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
      
  PLLCON = 1;				/* Enable PLL, disconnected */
  PLLFEED = 0xaa;
  PLLFEED = 0x55;

  CCLKCFG = CCLKDivValue;	/* Set clock divider */
#if USE_USB
  USBCLKCFG = USBCLKDivValue;		/* usbclk = 288 MHz/6 = 48 MHz */
#endif

  while ( ((PLLSTAT & (1 << 26)) == 0) );	/* Check lock bit status */
    
  MValue = PLLSTAT & 0x00007FFF;
  NValue = (PLLSTAT & 0x00FF0000) >> 16;
  while ((MValue != PLL_MValue) && ( NValue != PLL_NValue) );

  PLLCON = 3;				/* enable and connect */
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
  while ( ((PLLSTAT & (1 << 25)) == 0) );	/* Check connect bit status */
  return;
}

/******************************************************************************
** Function name:		TargetResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**						function; User may change it as needed, but may not 
**						deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetResetInit(void)
{
	MEMMAP = 0x1;			/* remap to internal flash */

#if USE_USB
	PCONP |= 0x80000000;		/* Turn On USB PCLK */
#endif
	/* Configure PLL, switch from IRC to Main OSC */
	ConfigurePLL();

	/* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
	PCLKSEL0 = 0x00000000;	/* PCLK is 1/4 CCLK */
	PCLKSEL1 = 0x00000000;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
	PCLKSEL0 = 0xAAAAAAAA;	/* PCLK is 1/2 CCLK */
	PCLKSEL1 = 0xAAAAAAAA;	 
#endif
#if (Fpclk / (Fcclk / 4)) == 4
	PCLKSEL0 = 0x55555555;	/* PCLK is the same as CCLK */
	PCLKSEL1 = 0x55555555;	
#endif

	/* Set memory accelerater module*/
	MAMCR = 0;
#if Fcclk < 20000000
	MAMTIM = 1;
#else
#if Fcclk < 40000000
	MAMTIM = 2;
#else
	MAMTIM = 3;
#endif
#endif
	MAMCR = 2;

	GPIOResetInit();

	return;
}


/*@}*/
